#include "ITransform.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

ITransform::ITransform(glm::vec3 position)
{
	this->position = position;
	pitch = yaw = roll = 0.f;

	forward = glm::vec3(0.f, 0.f, 1.f);
	right = glm::vec3(1.f, 0.f, 0.f);
	up = glm::vec3(1.f, 0.f, 0.f);
}

void ITransform::updateTransform()
{
	glm::mat4 orientationMatrix = glm::mat4_cast(glm::fquat(glm::vec3(pitch, yaw, roll)));

	forward = glm::normalize(glm::vec3(orientationMatrix * glm::vec4(0.f, 0.f, 1.f, 0.f)));
	right = glm::normalize(glm::vec3(orientationMatrix * glm::vec4(1.f, 0.f, 0.f, 0.f)));
	up = glm::normalize(glm::vec3(orientationMatrix * glm::vec4(0.f, 1.f, 0.f, 0.f)));
}

glm::mat4 ITransform::getWorldMatrix()
{
	glm::mat4 worldMatrix(1.f);

	worldMatrix = glm::translate(worldMatrix, position);

	worldMatrix = glm::rotate(worldMatrix, yaw, glm::vec3(0.f, 1.f, 0.f));
	worldMatrix = glm::rotate(worldMatrix, pitch, glm::vec3(1.f, 0.f, 0.f));
	worldMatrix = glm::rotate(worldMatrix, roll, glm::vec3(0.f, 0.f, 1.f));

	return worldMatrix;
}

glm::vec3 ITransform::getPosition()
{
	return position;
}

glm::vec3 ITransform::getForward()
{
	return forward;
}

glm::vec3 ITransform::getRight()
{
	return right;
}

glm::vec3 ITransform::getUp()
{
	return up;
}

float ITransform::getPitch()
{
	return pitch;
}

void ITransform::setPitch(float pitch)
{
	this->pitch = pitch;
}

float ITransform::getYaw()
{
	return yaw;
}

void ITransform::setYaw(float yaw)
{
	this->yaw = yaw;
}

float ITransform::getRoll()
{
	return roll;
}

void ITransform::setRoll(float roll)
{
	this->roll = roll;
}