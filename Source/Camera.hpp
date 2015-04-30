#pragma once

#include <glm.hpp>

class Camera
{
private:
	static const float MOVEMENT_SPEED;

	static float mouseSensitivity;

	glm::vec4 position;
	float pitch, yaw, roll;
	glm::vec4 forward, right, up;

	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool crouchKeyPressed;

	bool debugMode;
	bool flashLight;

public:
	Camera();

	void rotatePitch(float amount);
	void rotateYaw(float amount);
	void rotateRoll(float amount);
	
	void update(float delta);

	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();
	
	void setForwardKeyPressed(bool pressed);
	void setBackKeyPressed(bool pressed);
	void setLeftKeyPressed(bool pressed);
	void setRightKeyPressed(bool pressed);
	void setCrouchKeyPressed(bool pressed);

	void toggleDebugMode();
	bool isDebugModeOn();

	void toggleFlashLight();
	bool isFlashLightOn();
};