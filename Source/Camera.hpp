#pragma once

#include <glm.hpp>

class Camera
{
private:
	static const float MOVEMENT_SPEED;

	static float mouseSensitivity;

	glm::vec4 position;
	float pitch, yaw, roll;

	bool forwardKeyPressed = false;
	bool backKeyPressed = false;
	bool leftKeyPressed = false;
	bool rightKeyPressed = false;
	bool crouchKeyPressed = false;

public:
	Camera();

	void rotatePitch(float amount);
	void rotateYaw(float amount);
	void rotateRoll(float amount);
	
	glm::mat4 getViewMatrix(float delta);
	
	void setForwardKeyPressed(bool pressed);
	void setBackKeyPressed(bool pressed);
	void setLeftKeyPressed(bool pressed);
	void setRightKeyPressed(bool pressed);
	void setCrouchKeyPressed(bool pressed);
};