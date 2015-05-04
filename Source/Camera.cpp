#include "Camera.hpp"
#include "Util.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

const float Camera::MOVEMENT_SPEED = 5.f;

float Camera::mouseSensitivity = 0.4f;

Camera::Camera()
{
	position = glm::vec4(0.f, 100.f, 0.f, 1.f);
	pitch = yaw = roll = 0.f;

	forward = glm::vec4(0.f, 0.f, 1.f, 0.f);
	right = glm::vec4(1.f, 0.f, 0.f, 0.f);
	up = glm::vec4(1.f, 0.f, 0.f, 0.f);

	nearClipPlane = 1.f;
	farClipPlane = 10000.f;
	fov = 90.f;

	viewMatrix = projectionMatrix = glm::mat4(1.f);
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

void Camera::update(Input &input, float delta, unsigned int screenWidth, unsigned int screenHeight)
{
	glm::mat4 orientationMatrix = glm::mat4_cast(glm::fquat(glm::vec3(pitch, yaw, roll)));

	forward = glm::normalize(orientationMatrix * glm::vec4(0.f, 0.f, 1.f, 0.f));
	right = glm::normalize(orientationMatrix * glm::vec4(1.f, 0.f, 0.f, 0.f));
	up = glm::normalize(orientationMatrix * glm::vec4(0.f, 1.f, 0.f, 0.f));

	if (input.isForwardKeyPressed() && !input.isBackKeyPressed())
		position += forward * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);
	else if (input.isBackKeyPressed() && !input.isForwardKeyPressed())
		position -= forward * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);

	if (input.isLeftKeyPressed() && !input.isRightKeyPressed())
		position += right * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);
	else if (input.isRightKeyPressed() && !input.isLeftKeyPressed())
		position -= right * MOVEMENT_SPEED * delta * (input.isCrouchKeyPressed() ? .25f : 1.f);

	viewMatrix = glm::lookAt(glm::vec3(position), glm::vec3(position + forward), glm::vec3(up));

	float verticalFOV = Util::convertHorizontalToVerticalFOV(fov, (float)screenWidth, (float)screenHeight);
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	projectionMatrix = glm::perspective(glm::radians(verticalFOV / 2.f), aspectRatio, 1.0f, 10000.0f);
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

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}