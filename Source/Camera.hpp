#pragma once

#include "Input.hpp"
#include "ITransform.hpp"

class Camera : public ITransform
{
private:
	static const float MOVEMENT_SPEED;

	static float mouseSensitivity;

	float nearClipPlane, farClipPlane;
	float fov;

	glm::mat4 viewMatrix, projectionMatrix;

public:
	Camera(glm::vec3 position, unsigned int screenWidth, unsigned int screenHeight);

	void rotatePitch(float amount);
	void rotateYaw(float amount);
	void rotateRoll(float amount);
	
	void update(Input &input, float delta);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	float getFOV();
	void setFOV(float fov, unsigned int screenWidth, unsigned int screenHeight);
};