#pragma once

#include <vector>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

class LightManager
{
private:
	unsigned int selectedLightIndex;

public:
	std::vector<DirectionalLight*> lights;

	~LightManager();

	bool create();
	void update(float delta);

	void addDirectionalLight();
	void addPointLight();
	void addSpotLight();
	void deleteSelectedLight();

	inline int getSelectedLightIndex() const { return selectedLightIndex; }
	void selectLight(glm::vec2 mousePosition);
};