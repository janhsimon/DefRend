#pragma once

#include <glm.hpp>

class Transform
{
protected:
	glm::vec3 position, velocity, acceleration;
	float pitch, yaw, roll;
	glm::vec3 forward, right, up;

public:
	Transform(glm::vec3 position);

	void updateTransform(float delta);

	glm::mat4 getWorldMatrix();

	glm::vec3 getPosition();
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();

	float getPitch();
	void setPitch(float pitch);

	float getYaw();
	void setYaw(float yaw);

	float getRoll();
	void setRoll(float roll);
};