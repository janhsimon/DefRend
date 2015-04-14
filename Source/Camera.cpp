#include "Camera.hpp"

#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>

const float Camera::MOVEMENT_SPEED = 5.f;

float Camera::mouseSensitivity = 0.4f;

Camera::Camera()
{
	position = glm::vec4(0.f, 0.f, 0.f, 1.f);
	pitch = yaw = roll = 0.f;
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

glm::mat4 Camera::getViewMatrix(float delta)
{
	glm::mat4 orientationMatrix = glm::mat4_cast(glm::fquat(glm::vec3(pitch, yaw, roll)));

	glm::vec4 forward(0.f, 0.f, 1.f, 0.f);
	forward = glm::normalize(orientationMatrix * forward);

	glm::vec4 right(1.f, 0.f, 0.f, 0.f);
	right = glm::normalize(orientationMatrix * right);

	glm::vec4 up(0.f, 1.f, 0.f, 0.f);
	up = glm::normalize(orientationMatrix * up);

	if (forwardKeyPressed && !backKeyPressed)
		position += forward * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
	else if (backKeyPressed && !forwardKeyPressed)
		position -= forward * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
	
	if (leftKeyPressed && !rightKeyPressed)
		position += right * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);
	else if (rightKeyPressed && !leftKeyPressed)
		position -= right * MOVEMENT_SPEED * delta * (crouchKeyPressed ? .25f : 1.f);

	glm::vec3 p(position.x, position.y, position.z);
	glm::vec3 f(forward.x, forward.y, forward.z);
	glm::vec3 u(up.x, up.y, up.z);

	return glm::lookAt(p, p + f, u);
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