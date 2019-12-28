#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Time.h"
#include "Camera.h"

#define GRAVITY glm::vec3(0.0f, -9.81f, 0.0f)

class Player
{
public:
	Camera cam;
	void jump(float pwr);
	void updatePlayer();

private:
	void velocityUpdate();
	glm::vec3 velocity;
	bool grounded = true;

	float fixedTime = 0.2f; // this is basically unity's fixed timestep, it cannot be replaced by deltaTime as it needs to be a lot more consistent
};

