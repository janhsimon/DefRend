#pragma once

#include "..\Transform\Transform.hpp"

enum LightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};

class DirectionalLight : public Transform
{
public:
	glm::vec3 diffuseColor;
	float diffuseIntensity;
	LightType type;

	DirectionalLight();
};