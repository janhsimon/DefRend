#pragma once

#include "Input.hpp"

#include <glm.hpp>

class Camera
{
private:
	static const float MOVEMENT_SPEED;

	static float mouseSensitivity;

	glm::vec4 position;
	float pitch, yaw, roll;
	glm::vec4 forward, right, up;

	float nearClipPlane, farClipPlane;
	float fov;

	glm::mat4 viewMatrix, projectionMatrix;

public:
	Camera();

	void rotatePitch(float amount);
	void rotateYaw(float amount);
	void rotateRoll(float amount);
	
	void update(Input &input, float delta, unsigned int screenWidth, unsigned int screenHeight);

	glm::vec3 getPosition();
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};