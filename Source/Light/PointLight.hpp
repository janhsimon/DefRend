#pragma once

#include "DirectionalLight.hpp"
#include "..\Renderer\Cubemap.hpp"

class PointLight : public DirectionalLight
{
public:
	float specularIntensity;
	float specularPower;
	float shadowBias;

	Cubemap *shadowMap;
	
	//glm::vec3 attenuation; // constant, linear, exponent

	~PointLight();

	bool create();
	void update(float delta);
};