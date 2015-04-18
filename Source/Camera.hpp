#pragma once

#include <glm.hpp>

class Camera
{
private:
	static const float MOVEMENT_SPEED;

	static float mouseSensitivity;

	glm::vec4 position;
	float pitch, yaw, roll;

	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool crouchKeyPressed;

	bool debugMode;

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

	void toggleDebugMode();
	bool isDebugModeOn();
};