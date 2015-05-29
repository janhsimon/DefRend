#pragma once

#include "..\Model\Model.hpp"

class SceneManager
{
public:
	std::vector<Model*> models;

	~SceneManager();

	bool addModel(std::string filename, glm::vec3 position = glm::vec3(0.f));
};