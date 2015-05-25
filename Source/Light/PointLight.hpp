#pragma once

//#include "..\Renderer\Cubemap.hpp"
#include "DirectionalLight.hpp"

class PointLight : public DirectionalLight
{
private:
	//Cubemap *shadowMap;

public:
	float specularIntensity;
	float specularPower;
	
	glm::vec3 attenuation; // constant, linear, exponent

	~PointLight();

	bool create();
	void update(float delta);
	//void beginDrawingToShadowMap();
};