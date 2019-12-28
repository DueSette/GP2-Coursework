#include "Game.h"
#include "Time.h"

Game::Game()
	:_gameState(GameState::PLAY)
{}

Game::~Game()
{
}

void Game::run()
{
	init();
	gameLoop();
}

void Game::init()
{
	_gameDisplay.initialise();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	counter = 0.0f;

	_map.initialise(s_kModels + "map.obj", s_kTextures + "water.jpg", s_kShaders + "vertex_regular.shader", s_kShaders + "fragment.shader", glm::vec3(0, -1, 0), ColliderType::BOX);
	_map.setScale(glm::vec3(20, 20, 20));
	_map.setPosition(-VECTOR_UP * 5.0f);

	_dol0.initialise(s_kModels + "dolf.obj", s_kTextures + "pearly.png", s_kShaders + "vertex_scrollTexture.shader", s_kShaders + "fragment.shader", glm::vec3(0, 0, 15), ColliderType::NONE);
	_dol0.setRotation(glm::vec3(0, 0, 0));

	_dol1.initialise(s_kModels + "dolf.obj", s_kTextures + "hypnotic.png", s_kShaders + "vertex_scrollTexture.shader", s_kShaders + "fragment.shader", glm::vec3(0, 0, 15), ColliderType::NONE);
	_dol1.setRotation(glm::vec3(0, 90, 0));

	_dol2.initialise(s_kModels + "dolf.obj", s_kTextures + "grid.png", s_kShaders + "vertex_scrollTexture.shader", s_kShaders + "fragment.shader", glm::vec3(0, 0, 15), ColliderType::NONE);
	_dol2.setRotation(glm::vec3(0, -90, 0));

	dolphins.push_back(&_dol0);
	dolphins.push_back(&_dol1);
	dolphins.push_back(&_dol2);
	gameObjectList.push_back(&_map);

	//SOUND
	bangSFX = audioSource.loadSound("..\\res\\audio\\dolphin.wav");
	backGroundMusic = audioSource.loadSound("..\\res\\audio\\vapor.wav");
	audioSource.setlistener(VECTOR_ZERO, VECTOR_FORWARD);
	
	_player.cam.initialiseCamera
	(
		vec3(0.0f, 2.0f, -10.0f),
		70.0f,
		(float)_gameDisplay.getInfo().width / _gameDisplay.getInfo().height,
		0.01f,
		1000.0f
	);
}

void Game::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{		
		counter += 0.01f;
		++frames;

		deltaTime = (float)updateDeltaTime();

		inputUpdate();
		physicsLoop();
		renderLoop();

		playAudio(backGroundMusic, VECTOR_ZERO);
	}
}

void Game::inputUpdate()
{
	SDL_Event e;
	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			if (keyboard_state[SDL_SCANCODE_W])
			{
				camSpeedZ = 17.0f * deltaTime;
			}

			if (keyboard_state[SDL_SCANCODE_S])
			{
				camSpeedZ = -17.0f * deltaTime;
			}

			if (keyboard_state[SDL_SCANCODE_A])
			{
				camSpeedX = -17.0f * deltaTime;
			}
			if (keyboard_state[SDL_SCANCODE_D])
			{
				camSpeedX = 17.0f * deltaTime;
			}
		}

		else if (e.type == SDL_KEYUP && e.key.repeat == 0)
		{
			if (!keyboard_state[SDL_SCANCODE_W])
			{
				if (camSpeedZ > 0)
					camSpeedZ = 0;
			}

			if (!keyboard_state[SDL_SCANCODE_S])
			{
				if (camSpeedZ < 0)
					camSpeedZ = 0;
			}

			if (!keyboard_state[SDL_SCANCODE_A])
			{
				if (camSpeedX < 0)
					camSpeedX = 0;
			}
			if (!keyboard_state[SDL_SCANCODE_D])
			{
				if (camSpeedX > 0)
					camSpeedX = 0;
			}
		}

		switch (e.type)
		{
			//=======================IN CASE OF QUITTING
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

			//=======================IN CASE OF MOUSE MOV
		case SDL_MOUSEMOTION:
		{
			_player.cam.pitch(e.motion.yrel * deltaTime * 0.3f); //rotates up and down
			_player.cam.yaw(e.motion.xrel * deltaTime * -0.3f); //rotates left and right
			break;
		}
		break;
		case SDL_KEYUP:
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
				_player.jump(0.3f);
				break;

			case SDLK_0:
				GameObject* g = new GameObject;
				g->initialise("..\\res\\models\\long lab table.obj", s_kTextures + "grid.png", s_kShaders + "vertex_regular.shader", s_kShaders + "fragment.shader", glm::vec3(0, 0, 15), ColliderType::SPHERE);
				gameObjectList.push_back(g);
				audioSource.playSound(bangSFX);
			}
			break;
		}

		case SDL_MOUSEWHEEL:
			_player.cam.moveOnZ(e.wheel.y * 4.0f);
			break;
		}
	}

	_player.cam.moveOnX(camSpeedX);
	_player.cam.moveOnZ(camSpeedZ);
}

void Game::playAudio(unsigned int Source, glm::vec3 pos)
{
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);

	if (AL_PLAYING != state)
		audioSource.playSound(Source, pos);
}

void Game::physicsLoop()
{
	// ===== COLLISIONS
	_player.updatePlayer();

	for (int i = 0; i < gameObjectList.size(); i++)
	{
		GameObject* g1 = gameObjectList[i];
		Collider* colA = g1->getCollider();
		BoxCollider* cA = dynamic_cast<BoxCollider*> (colA);

		cA->getSize();
		if (colA == nullptr)		
			continue;
		
		for (int l = 0; l < gameObjectList.size(); l++)
		{
			GameObject* g2 = gameObjectList[l];
			Collider* colB = g2->getCollider();

			if (colB == nullptr)
				continue;

			if (colA != colB)
			{
				if (checkCollisions(colA->getSize(), colB->getSize(), g1->getPosition(), g2->getPosition()))
					std::cout << colA << " collided with " << colB << std::endl;
				else
					std::cout << colA << " did not collide with " << colB << std::endl;
			}
		}
	}
}

void Game::renderLoop()
{
	_gameDisplay.clearDisplay(cos(counter) / 2 + 0.55f, -cos(counter) / 4 + 0.25f, 0.4f, 1);

	for (int j = 0; j < dolphins.size(); j++)
	{
		dolphins[j]->translate(glm::vec3(1 * cos(counter), 1 * sin(counter), 0) * deltaTime);
		dolphins[j]->rotate(VECTOR_RIGHT * deltaTime);
		dolphins[j]->drawProcedure(_player.cam); //that's how we render every object in the scene
	}

	for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->drawProcedure(_player.cam);
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();
	_gameDisplay.swapBuffer();
}

bool Game::checkCollisions(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2)
{
	float distance = (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z);
	distance = SDL_sqrt(distance);

	return distance > rad1 + rad2 ? false : true;
}

bool Game::checkCollisions(glm::vec3 s1, glm::vec3 s2, glm::vec3& pos1, glm::vec3& pos2)
{
	if (abs(pos1.x - pos2.x) < s1.x + s2.x)
		if (abs(pos1.y - pos2.y) < s1.y + s2.y)
			return (abs(pos1.z - pos2.z) < s1.z + s2.z);
}
