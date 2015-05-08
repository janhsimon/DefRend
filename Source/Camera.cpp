#include "Camera.hpp"
#include "Util.hpp"

#include <gtc/matrix_transform.hpp>

const float Camera::MOVEMENT_SPEED = 5.f;

float Camera::mouseSensitivity = .4f;

Camera::Camera(glm::vec3 position, unsigned int screenWidth, unsigned int screenHeight) : Transform(position)
{
	nearClipPlane = 1.f;
	farClipPlane = 10000.f;

	setFOV(90.f, screenWidth, screenHeight);

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

	float pitchLock = glm::radians(90.f);

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
	updateTransform();

	if (input.isForwardKeyPressed() && !input.isBackKeyPressed())
		position += forward * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);
	else if (input.isBackKeyPressed() && !input.isForwardKeyPressed())
		position -= forward * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);

	if (input.isLeftKeyPressed() && !input.isRightKeyPressed())
		position += right * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);
	else if (input.isRightKeyPressed() && !input.isLeftKeyPressed())
		position -= right * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);

	viewMatrix = glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

float Camera::getFOV()
{
	return fov;
}

void Camera::setFOV(float fov, unsigned int screenWidth, unsigned int screenHeight)
{
	this->fov = fov;
	
	float verticalFOV = Util::convertHorizontalToVerticalFOV(fov, (float)screenWidth, (float)screenHeight);
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	projectionMatrix = glm::perspective(glm::radians(verticalFOV / 2.f), aspectRatio, 1.0f, 10000.0f);
}