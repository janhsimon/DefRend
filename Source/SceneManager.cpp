#include "SceneManager.hpp"
#include "Util.hpp"

SceneManager::~SceneManager()
{
	for (Model *model : models)
		delete model;
}

bool SceneManager::addModel(std::string filename, glm::vec3 position)
{
	Model *newModel;

	if (!Util::checkMemory(newModel = new Model(position)))
		return false;

	if (!newModel->load(filename))
		return false;

	models.push_back(newModel);

	return true;
}

std::vector<Model*> *SceneManager::getModels()
{
	return &models;
}