#pragma once

#include "DirectionalLight.hpp"
#include "..\Renderer\Cubemap.hpp"

class PointLight : public DirectionalLight
{
private:
	bool castShadows;

public:
	float specularIntensity;
	float specularPower;
	float shadowBias;

	Cubemap *shadowMap;
	
	//glm::vec3 attenuation; // constant, linear, exponent

	~PointLight();

	bool create();
	void update(float delta);

	inline bool getCastShadows() const { return castShadows; }
	void setCastShadows(bool castShadows);
};