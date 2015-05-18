#pragma once

#include "..\Model\Model.hpp"

class SceneManager
{
private:
	std::vector<Model*> models;

public:
	~SceneManager();

	bool addModel(std::string filename, glm::vec3 position);
	std::vector<Model*> *getModels();
};