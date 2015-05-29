#include <gtc\matrix_transform.hpp>

#include "Camera.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

const float Camera::MOVEMENT_SPEED = 5.f;
const float Camera::CROUCH_SPEED_FACTOR = .25f;

float Camera::mouseSensitivity = 30.f;

extern Window *window;

Camera::Camera(glm::vec3 position) : Transform(position)
{
	nearClipPlane = 1.f;
	farClipPlane = 10000.f;

	setFOV(90.f, window->width, window->height);
	setFirstPerson(true);

	viewMatrix = glm::mat4(1.f);
}

void *Camera::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void Camera::operator delete(void *p)
{
	_aligned_free(p);
}

void Camera::rotatePitch(float amount)
{
	pitch += amount * mouseSensitivity;

	float pitchLock = 90.f;

	if (pitch < -pitchLock)
		pitch = -pitchLock;
	else if (pitch > pitchLock)
		pitch = pitchLock;
}

void Camera::rotateYaw(float amount)
{
	yaw -= amount * mouseSensitivity;
}

void Camera::rotateRoll(float amount)
{
	roll += amount * mouseSensitivity;
}

void Camera::update(Input &input, float delta)
{
	//acceleration = glm::vec3(0.f, 0.f, 0.f);

	if (input.forwardKeyPressed && !input.backKeyPressed)
		position += getForward() * MOVEMENT_SPEED * delta * (input.crouchKeyPressed ? CROUCH_SPEED_FACTOR : 1.f);
	else if (input.backKeyPressed && !input.forwardKeyPressed)
		position -= getForward() * MOVEMENT_SPEED * delta * (input.crouchKeyPressed ? CROUCH_SPEED_FACTOR : 1.f);

	if (input.leftKeyPressed && !input.rightKeyPressed)
		position += getRight() * MOVEMENT_SPEED * delta * (input.crouchKeyPressed ? CROUCH_SPEED_FACTOR : 1.f);
	else if (input.rightKeyPressed && !input.leftKeyPressed)
		position -= getRight() * MOVEMENT_SPEED * delta * (input.crouchKeyPressed ? CROUCH_SPEED_FACTOR : 1.f);

	updateTransform(delta);

	viewMatrix = glm::lookAt(position, position + getForward(), getUp());
}

void Camera::setFOV(float fov, unsigned int screenWidth, unsigned int screenHeight)
{
	this->fov = fov;
	
	float verticalFOV = Util::convertHorizontalToVerticalFOV(fov, (float)screenWidth, (float)screenHeight);
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	projectionMatrix = glm::perspective(glm::radians(verticalFOV / 2.f), aspectRatio, 1.0f, 10000.0f);
}

void Camera::setFirstPerson(bool firstPerson)
{
	this->firstPerson = firstPerson;
	SDL_ShowCursor(!firstPerson);

	if (firstPerson)
		SDL_WarpMouseInWindow(window->sdlWindow, window->width / 2, window->height / 2);
}