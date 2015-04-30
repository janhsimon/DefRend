#include "Camera.hpp"

#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>

const float Camera::MOVEMENT_SPEED = 5.f;

float Camera::mouseSensitivity = 0.4f;

Camera::Camera()
{
	position = glm::vec4(0.f, 100.f, 0.f, 1.f);
	pitch = yaw = roll = 0.f;

	forwardKeyPressed = backKeyPressed = leftKeyPressed = rightKeyPressed = crouchKeyPressed = false;

	debugMode = false;
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

void Camera::update(float delta)
{
	glm::mat4 orientationMatrix = glm::mat4_cast(glm::fquat(glm::vec3(pitch, yaw, roll)));

	glm::vec4 f(0.f, 0.f, 1.f, 0.f);
	forward = glm::normalize(orientationMatrix * f);

	glm::vec4 r(1.f, 0.f, 0.f, 0.f);
	right = glm::normalize(orientationMatrix * r);

	glm::vec4 u(0.f, 1.f, 0.f, 0.f);
	up = glm::normalize(orientationMatrix * u);

	if (forwardKeyPressed && !backKeyPressed)
		position += forward * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
	else if (backKeyPressed && !forwardKeyPressed)
		position -= forward * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);

	if (leftKeyPressed && !rightKeyPressed)
		position += right * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
	else if (rightKeyPressed && !leftKeyPressed)
		position -= right * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
}

glm::mat4 Camera::getViewMatrix()
{
	glm::vec3 p(position.x, position.y, position.z);
	glm::vec3 f(forward.x, forward.y, forward.z);
	glm::vec3 u(up.x, up.y, up.z);

	return glm::lookAt(p, p + f, u);
}

glm::vec3 Camera::getPosition()
{
	return glm::vec3(position);
}

glm::vec3 Camera::getForward()
{
	return glm::vec3(forward);
}

glm::vec3 Camera::getRight()
{
	return glm::vec3(right);
}

glm::vec3 Camera::getUp()
{
	return glm::vec3(up);
}

void Camera::setForwardKeyPressed(bool pressed)
{
	forwardKeyPressed = pressed;
}

void Camera::setBackKeyPressed(bool pressed)
{
	backKeyPressed = pressed;
}

void Camera::setLeftKeyPressed(bool pressed)
{
	leftKeyPressed = pressed;
}

void Camera::setRightKeyPressed(bool pressed)
{
	rightKeyPressed = pressed;
}

void Camera::setCrouchKeyPressed(bool pressed)
{
	crouchKeyPressed = pressed;
}

void Camera::toggleDebugMode()
{
	debugMode = !debugMode;
}

bool Camera::isDebugModeOn()
{
	return debugMode;
}

void Camera::toggleFlashLight()
{
	flashLight = !flashLight;
}

bool Camera::isFlashLightOn()
{
	return flashLight;
}