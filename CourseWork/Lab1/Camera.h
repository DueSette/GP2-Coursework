#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Time.h"

using namespace glm;

class Camera
{
public:
	Camera()
	{}

	void initialiseCamera(const vec3& pos, float fov, float aspect, float nearClip, float farClip);

	mat4 GetViewProjection() const;

	void moveOnZ(float val);
	void moveOnX(float val);
	
	inline void yaw(float angle) //rotates around Y axis (goes left and right)
	{
		static const vec3 UP(0.0f, 1.0f, 0.0f);

		mat4 rotation = rotate(angle, UP);

		forward = vec3(normalize(rotation * vec4(forward, 0.0)));
		up = vec3(normalize(rotation * vec4(up, 0.0)));
	}

	inline void pitch(float angle) //rotates around X axis (goes up and down)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	inline glm::vec3 getPosition() { return pos; }

	inline void setPosition(glm::vec3 newPos) { pos = newPos; }

	inline void translate(glm::vec3 offset) { pos += offset; }

private:
	mat4 projection;

	vec3 pos;
	vec3 forward;
	vec3 up;
};

