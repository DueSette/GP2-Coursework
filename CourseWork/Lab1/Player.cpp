#include "Player.h"

Player::Player()
{
	init();
}

void Player::init()
{
	audioManager.setlistener(VECTOR_ZERO, VECTOR_FORWARD);
	landSound = audioManager.loadSound("..\\res\\audio\\Land.wav");
	jumpSound = audioManager.loadSound("..\\res\\audio\\Jump.wav");
}

void Player::jump(float pwr) //pwr is the amount in vertical units that we want to jump
{
	if (grounded)
	{
		float j = glm::sqrt(pwr * -2 * GRAVITY.y); //standard jump formula
		velocity.y += j;
		
		grounded = false;
		audioManager.playSound(jumpSound);
	}
}

void Player::updatePlayer()
{	
	velocityUpdate();

	cam.translate(velocity);
	if (cam.getPosition().y < 2.0f)
	{
		velocity.y = 0.0f;
		cam.setPosition(glm::vec3(cam.getPosition().x, 2.0f, cam.getPosition().z)); //reset on the Y in case it compenetrates a little
		grounded = true;
		audioManager.playSound(landSound);
	}
}

void Player::velocityUpdate()
{
	float subjectiveGravity;

	if (velocity.y > 0.0f)
		subjectiveGravity = -4.65f;
	else
		subjectiveGravity = -6.8f;

	if (cam.getPosition().y > 2.0f)	//slow player down if not "grounded"
		velocity.y += subjectiveGravity * (fixedTime * fixedTime);
}
