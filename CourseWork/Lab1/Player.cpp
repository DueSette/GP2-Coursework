#include "Player.h"

void Player::jump(float pwr) //pwr is the amount in vertical units that we want to jump
{
	if (grounded)
	{
		float j = glm::sqrt(pwr * -2 * GRAVITY.y); //standard jump formula
		velocity.y += j;
		
		grounded = false;
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
	}
}

void Player::velocityUpdate()
{
	float subjectiveGravity;

	if (velocity.y > 0.0f)
		subjectiveGravity = -5.2f;
	else
		subjectiveGravity = -6.8f;

	if (cam.getPosition().y > 2.0f)	//slow player down if not "grounded"
		velocity.y += subjectiveGravity * (fixedTime * fixedTime);
	else
		grounded = true;
}
