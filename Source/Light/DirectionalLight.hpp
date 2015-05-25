#pragma once

#include "..\Transform\Transform.hpp"

class DirectionalLight : public Transform
{
public:
	glm::vec3 diffuseColor;
	float diffuseIntensity;
};