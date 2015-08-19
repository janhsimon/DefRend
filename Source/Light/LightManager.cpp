#include <assert.h>
#include <sstream>

#include "LightManager.hpp"
#include "..\Camera\Camera.hpp"
#include "..\Input\InputManager.hpp"
#include "..\UI\LightEditor.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

#include "gtc\matrix_transform.hpp"

extern Camera *camera;
extern InputManager *inputManager;
extern LightEditor *lightEditor;
extern Window *window;

LightManager::~LightManager()
{
	for (DirectionalLight *d : lights)
		delete d;
}

bool LightManager::create()
{
	selectedLightIndex = -1;

	
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

	/*
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
	lights.push_back(p);

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
	*/

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

	if (selectedLightIndex < 0 || selectedLightIndex >= lights.size())
		return;

	if (inputManager->lightForwardKeyPressed && !inputManager->lightBackKeyPressed)
		lights[selectedLightIndex]->position += camera->getForward() * delta;
	else if (inputManager->lightBackKeyPressed && !inputManager->lightForwardKeyPressed)
		lights[selectedLightIndex]->position -= camera->getForward() * delta;

	if (inputManager->lightLeftKeyPressed && !inputManager->lightRightKeyPressed)
		lights[selectedLightIndex]->position += camera->getRight() * delta;
	else if (inputManager->lightRightKeyPressed && !inputManager->lightLeftKeyPressed)
		lights[selectedLightIndex]->position -= camera->getRight() * delta;

	if (inputManager->lightUpKeyPressed && !inputManager->lightDownKeyPressed)
		lights[selectedLightIndex]->position += camera->getUp() * delta;
	else if (inputManager->lightDownKeyPressed && !inputManager->lightUpKeyPressed)
		lights[selectedLightIndex]->position -= camera->getUp() * delta;
}

void LightManager::addDirectionalLight()
{
	DirectionalLight *d;

	if (!Util::checkMemory(d = new DirectionalLight()))
	{
		Error::report("Error", "Failed to allocate memory for new directional light.");
		return;
	}

	d->position = camera->position;
	d->setYaw(camera->getYaw());
	d->setPitch(camera->getPitch());
	d->setRoll(camera->getRoll());
	d->diffuseColor = glm::vec3(1.f);
	d->diffuseIntensity = 1.f;
	lights.push_back(d);
}

void LightManager::addPointLight()
{
	PointLight *p;

	if (!Util::checkMemory(p = new PointLight()))
	{
		Error::report("Error", "Failed to allocate memory for new point light.");
		return;
	}

	if (!p->create())
	{
		Error::report("Error", "Failed to allocate memory for the cubemap for new point light.");
		return;
	}

	p->create();

	p->position = camera->position;
	p->diffuseColor = glm::vec3(1.f);
	p->diffuseIntensity = 1000.f;
	p->specularIntensity = 1000.f;
	p->specularPower = 4.f;
	p->shadowBias = .99f;
	lights.push_back(p);
}

void LightManager::addSpotLight()
{
	SpotLight *s;

	if (!Util::checkMemory(s = new SpotLight()))
	{
		Error::report("Error", "Failed to allocate memory for new spot light.");
		return;
	}

	s = new SpotLight();

	s->position = camera->position;
	s->diffuseColor = glm::vec3(1.f);
	s->diffuseIntensity = 1000.f;
	s->specularIntensity = 1000.f;
	s->specularPower = 32.f;
	s->setYaw(camera->getYaw());
	s->setPitch(camera->getPitch());
	s->setRoll(camera->getRoll());
	s->cutoffAngle = 25.f;
	lights.push_back(s);
}

void LightManager::deleteSelectedLight()
{
	if (selectedLightIndex < 0 || selectedLightIndex >= lights.size())
		return;
	
	lights.erase(lights.begin() + selectedLightIndex);
	selectedLightIndex = -1;
}

void LightManager::setShadowsForSelectedLight(bool toggle)
{
	if (selectedLightIndex < 0 || selectedLightIndex >= lights.size())
		return;

	DirectionalLight *l = lights[selectedLightIndex];

	if (l->type != LightType::POINT_LIGHT)
		return;

	PointLight *p = (PointLight*)l;

	p->setCastShadows(toggle);
}

void LightManager::selectLight(glm::vec2 mousePosition)
{
	assert(window);
	assert(camera);

	if (lights.size() <= 0)
		return;


	// bring mouse position into OpenGL screen space range (-1,1 top left)

	// [0,0] - [1,1]
	mousePosition.x /= window->width;
	mousePosition.y /= window->height;

	// [-1,-1] - [1,1]
	mousePosition = mousePosition * 2.f - 1.f;

	// flip y
	mousePosition.y = -mousePosition.y;

	glm::vec2 lightPos(lights[0]->position);

	for (unsigned int i = 0; i < lights.size(); ++i)
	{
		DirectionalLight *s = lights[i];

		glm::vec3 camToLight(s->position - camera->position);
		camToLight = glm::normalize(camToLight);

		if (glm::dot(camToLight, camera->getForward()) <= 0.f)
		// if the light is behind the camera
			// we can't select it
			continue;

		// bring the light position into screen space
		glm::vec4 p4 = camera->projectionMatrix * camera->viewMatrix * glm::vec4(s->position, 1.f);
		p4.x /= p4.w;
		p4.y /= p4.w;

		glm::vec2 p(p4);

		/*
		std::stringstream ss;1
		ss << "mousePos: " << mousePosition.x << " / " << mousePosition.y << "   lightPos: " << p.x << " / " << p.y;
		Error::report("Debug", ss.str());
		*/

		// get the screen space distance between the light position and the mouse cursor
		float distance = glm::length(glm::vec2(p - mousePosition));

		if (distance < /*.04f*/ .1f)
		// if the mouse was clicked close to the light
		{
			// select it and we're done
			selectedLightIndex = i;
			lightEditor->loadSliderValuesFromLight(lights[selectedLightIndex]);
			return;
		}
	}
}