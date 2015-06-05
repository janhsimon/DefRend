#pragma once

#include "..\Transform\Transform.hpp"

class Camera : public Transform
{
private:
	static const float MOVEMENT_SPEED;
	static const float CROUCH_SPEED_FACTOR;

	float fov;
	bool firstPerson;

public:
	glm::mat4 viewMatrix, projectionMatrix;
	float nearClipPlane, farClipPlane;
	static float mouseSensitivity;

	Camera(glm::vec3 position);

	// 16-byte alignment for fast SSE math instructions
	void *operator new(size_t size);
	void operator delete(void *p);

	void rotatePitch(float amount);
	void rotateYaw(float amount);
	void rotateRoll(float amount);
	
	void update(float delta);

	inline float getFOV() const { return fov; }
	void setFOV(float fov, unsigned int screenWidth, unsigned int screenHeight);

	inline bool getFirstPerson() const { return firstPerson; }
	void setFirstPerson(bool firstPerson);
};