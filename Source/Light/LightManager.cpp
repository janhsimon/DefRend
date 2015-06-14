#include <sstream>

#include "LightManager.hpp"
#include "..\Camera\Camera.hpp"
#include "..\Input\InputManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

#include "gtc\matrix_transform.hpp"

extern Camera *camera;
extern InputManager *inputManager;

LightManager::~LightManager()
{
	for (DirectionalLight *d : lights)
		delete d;
}

bool LightManager::create()
{
	selectedLightIndex = 1;


	// add a directional light

	DirectionalLight *d;

	if (!Util::checkMemory(d = new DirectionalLight()))
		return false;

	d->position = glm::vec3(0.f, 1200.f, 0.f);
	d->setYaw(45.f);
	d->setPitch(45.f);
	d->diffuseColor = glm::vec3(.7f, .7f, 1.f);
	d->diffuseIntensity = 1.f;
	lights.push_back(d);


	// add point lights
	
	PointLight *p;

	if (!Util::checkMemory(p = new PointLight()))
		return false;

	if (!p->create())
		return false;

	p->position = glm::vec3(0.f, 50.f, 0.f);
	p->diffuseColor = glm::vec3(1.f, 1.f, .9f);
	p->diffuseIntensity = 2000.f;
	p->specularIntensity = 2000.f;
	p->specularPower = 4.f;
	p->shadowBias = .99f;
	//p->attenuation = glm::vec3(.0001f, .00001f, .00001f);
	//p->attenuation = glm::vec3(0.0f, 0.0f, 1.f);
	lights.push_back(p);

	/*
	if (!Util::checkMemory(p = new PointLight()))
		return false;

	if (!p->create())
		return false;

	p->position = glm::vec3(300.f, 1000.f, 0.f);
	p->diffuseColor = glm::vec3(.1f, .1f, 1.f);
	p->diffuseIntensity = 512.f;
	p->specularIntensity = 512.f;
	p->specularPower = 32.f;
	//p->attenuation = glm::vec3(.0001f, .00001f, .00001f);
	pointLights.push_back(p);
	*/


	// and some spot lights

	for (int i = 0; i < 13; ++i)
	{
		SpotLight *s;
		
		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position = glm::vec3(1075.f - i * 189.f, 0.f, 560.f);
		s->diffuseColor = glm::vec3(1.f, 1.f, .9f);
		s->diffuseIntensity = 512.f;
		s->specularIntensity = 512.f;
		s->specularPower = 32.f;
		//s->attenuation = glm::vec3(.0001f, .00001f, .00001f);
		s->setPitch(-90.f);
		s->cutoffAngle = 25.f;
		lights.push_back(s);

		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position = glm::vec3(1075.f - i * 189.f, 0.f, -630.f);
		s->diffuseColor = glm::vec3(1.f, 1.f, .9f);
		s->diffuseIntensity = 512.f;
		s->specularIntensity = 512.f;
		s->specularPower = 32.f;
		//s->attenuation = glm::vec3(.0001f, .00001f, .00001f);
		s->setPitch(-90.f);
		s->cutoffAngle = 25.f;
		lights.push_back(s);
	}

	/*
	// also create a flashlight

	if (!Util::checkMemory(flashLight = new SpotLight()))
		return false;

	flashLight->diffuseColor = glm::vec3(.9f, .9f, 1.f);
	flashLight->diffuseIntensity = 1.f;
	flashLight->specularIntensity = 1.f;
	flashLight->specularPower = 32.f;
	flashLight->attenuation = glm::vec3(.0001f, .00001f, .00001f);
	flashLight->cutoffAngle = 30.f;
	spotLights.push_back(flashLight);
	*/

	return true;
}

void LightManager::update(float delta)
{
	/*
	static float z = 0.f;
	pointLights[0]->position.x = sinf(z) * 100.f;//* 1200.f;
	//pointLights[1]->position.y = 500.f + sinf(z) * 500.f;
	z += .005f * delta;
	*/

	if (inputManager->lightForwardKeyPressed && !inputManager->lightBackKeyPressed)
		lights[1]->position += lights[1]->getForward() * delta;
	else if (inputManager->lightBackKeyPressed && !inputManager->lightForwardKeyPressed)
		lights[1]->position -= lights[1]->getForward() * delta;

	if (inputManager->lightLeftKeyPressed && !inputManager->lightRightKeyPressed)
		lights[1]->position += lights[1]->getRight() * delta;
	else if (inputManager->lightRightKeyPressed && !inputManager->lightLeftKeyPressed)
		lights[1]->position -= lights[1]->getRight() * delta;

	if (inputManager->lightUpKeyPressed && !inputManager->lightDownKeyPressed)
		lights[1]->position += lights[1]->getUp() * delta;
	else if (inputManager->lightDownKeyPressed && !inputManager->lightUpKeyPressed)
		lights[1]->position -= lights[1]->getUp() * delta;
}

void LightManager::selectLight(glm::vec2 mousePosition)
{
	/*
	glm::vec3 pos = glm::vec3(mousePosition, 0.f);
    glm::vec4 viewport = glm::vec4(0.f, 0.f, window->width, window->height);
	glm::vec3 un = glm::unProject(pos, camera->viewMatrix, camera->projectionMatrix, viewport);

	std::stringstream s;
	s << "Unprojecting point at " << mousePosition.x << "/" << mousePosition.y << " = " << un.x << "/" << un.y << "/" << un.z;
	Error::report("Debug", s.str());
	*/

	float nearestDistance = glm::length(glm::vec3(lights[0]->position - camera->position));
	int nearestIndex = 0;

	for (unsigned int i = 1; i < lights.size(); ++i)
	{
		DirectionalLight *s = lights[i];
		float distance = glm::length(glm::vec3(s->position - camera->position));

		if (distance < nearestDistance)
		{
			nearestIndex = i;
			nearestDistance = distance;
		}
	}

	selectedLightIndex = nearestIndex;
}