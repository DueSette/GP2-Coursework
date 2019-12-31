#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ostream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Display.h"
#include "Shader.h"
#include "Transform.h"
#include "GameObject.h"
#include "Audio.h"
#include "Player.h"

enum class GameState { PLAY, EXIT, PAUSE };

static float deltaTime = 0.000f;
static float counter;
static int frames = 0;

class Game
{

public:
	Game();
	~Game();
	void run();

private:

	void init();
	void gameLoop();
	void physicsLoop();
	void renderLoop();

	bool checkCollisions(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2); //legacy
	static bool checkCollisions(glm::vec3 s1, glm::vec3 s2, glm::vec3& pos1, glm::vec3& pos2); //collider-agnostic collision algorithm

	void playAudio(unsigned int Source, glm::vec3 pos);

	Player _player;
	Camera _camera;
	Display _gameDisplay;
	GameState _gameState;

	std::vector<GameObject*> gameObjectList;
	std::vector<GameObject*> dolphins; //since they move in a specific way we put them in a separate list

	GameObject _map;

	GameObject _dol0;
	GameObject _dol1;
	GameObject _dol2;

	//Sound Stuff
	Audio audioSource;

	unsigned int bangSFX;
	unsigned int backGroundMusic;

	//INPUT
	void inputUpdate();

	float camSpeedX = 0.0f;
	float camSpeedZ = 0.0f;
};
