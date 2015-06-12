#pragma once

#include <vector>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

class LightManager
{
private:
	//SpotLight *flashLight;
	unsigned int selectedPointLight;

public:
	std::vector<DirectionalLight*> directionalLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	~LightManager();

	bool create();
	void update(float delta);

	inline int getSelectedPointLight() const { return selectedPointLight; }
	void selectPointLight(glm::vec2 mousePosition);
};