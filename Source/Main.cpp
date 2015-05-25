#include <sstream>

#include "Camera\Camera.hpp"
#include "Input\Input.hpp"
#include "Light\PointLight.hpp"
#include "Light\SpotLight.hpp"
#include "Renderer\BillboardRenderer.hpp"
#include "Renderer\DeferredRenderer.hpp"
#include "Renderer\FontRenderer.hpp"
#include "Renderer\IRenderer.hpp"
#include "Scene\SceneManager.hpp"
#include "Util\Error.hpp"
#include "Util\Util.hpp"
#include "Window\Window.hpp"

BillboardRenderer *billboardRenderer;
Camera *camera;
FontRenderer *fontRenderer;
IRenderer *renderer;
Input input;
SceneManager *sceneManager;
Window *window;

std::vector<DirectionalLight*> directionalLights;
std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;
//SpotLight *flashLight;

unsigned int pointLightSelected = -1;

unsigned int thisTickTime, lastTickTime = 0;

glm::mat4 projectionMatrix;

bool load()
{
	// create a window

	if (!Util::checkMemory(window = new Window()))
		return false;

	if (!window->create(1600, 900))
		return false;


	// check there was no error initializing OpenGL

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to initialize OpenGL: " + Error::getOpenGLErrorString(error));
		return false;
	}


	// initialize glew

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		Error::report("Error", "Failed to initialize GLEW.");
		return false;
	}

	if (!GLEW_VERSION_4_4)
	{
		Error::report("Error", "OpenGL v4.4 is required but your system does not support it.");
		return false;
	}

	error = glGetError();
	if (error != GL_NO_ERROR && error != GL_INVALID_ENUM)
	// ignore a possible invalid enum error after GLEW initialization, known OpenGL issue without fix :(
	{
		Error::report("Error", "Failed to initialize GLEW: " + Error::getOpenGLErrorString(error));
		return false;
	}


	// create font renderer

	if (!Util::checkMemory(billboardRenderer = new BillboardRenderer()))
		return false;

	if (!billboardRenderer->create())
		return false;


	// create deferred renderer

	if (!Util::checkMemory(renderer = new DeferredRenderer()))
		return false;

	if (!renderer->init(window))
		return false;


	// create font renderer

	if (!Util::checkMemory(fontRenderer = new FontRenderer()))
		return false;

	if (!fontRenderer->create())
		return false;


	// create camera

	if (!Util::checkMemory(camera = new Camera(glm::vec3(0.f, 100.f, 0.f), window->width, window->height)))
		return false;


	// create scene manager

	if (!Util::checkMemory(sceneManager = new SceneManager()))
		return false;


	// load models	

	if (!sceneManager->addModel("Models\\Sponza\\Sponza.obj", glm::vec3(0.f)))
		return false;

	if (!sceneManager->addModel("Models\\OldMan\\OldMan.obj", glm::vec3(-500.f, -3.f, 0.f)))
		return false;


	// add a directional light

	DirectionalLight *d;

	if (!Util::checkMemory(d = new DirectionalLight()))
		return false;

	d->position = glm::vec3(0.f, 1200.f, 0.f);
	d->setYaw(45.f);
	d->setPitch(45.f);
	d->diffuseColor = glm::vec3(.7f, .7f, 1.f);
	d->diffuseIntensity = 1.f;
	directionalLights.push_back(d);


	// add a point light
	
	PointLight *p;

	if (!Util::checkMemory(p = new PointLight()))
		return false;

	p->position = glm::vec3(0.f, 10.f, 0.f);
	p->diffuseColor = glm::vec3(1.f, 1.f, .9f);
	p->diffuseIntensity = 1.f;
	p->specularIntensity = 1.f;
	p->specularPower = 32.f;
	p->attenuation = glm::vec3(.0001f, .00001f, .00001f);
	pointLights.push_back(p);

	if (!Util::checkMemory(p = new PointLight()))
		return false;

	p->position = glm::vec3(300.f, 1000.f, 0.f);
	p->diffuseColor = glm::vec3(.1f, .1f, 1.f);
	p->diffuseIntensity = 1.f;
	p->specularIntensity = 1.f;
	p->specularPower = 32.f;
	p->attenuation = glm::vec3(.0001f, .00001f, .00001f);
	pointLights.push_back(p);


	// and some spot lights

	for (int i = 0; i < 13; ++i)
	{
		SpotLight *s;
		
		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position = glm::vec3(1075.f - i * 189.f, 0.f, 560.f);
		s->diffuseColor = glm::vec3(1.f, 1.f, .9f);
		s->diffuseIntensity = 2.f;
		s->specularIntensity = 1.f;
		s->specularPower = 32.f;
		s->attenuation = glm::vec3(.0001f, .00001f, .00001f);
		s->setPitch(-90.f);
		s->cutoffAngle = 25.f;
		spotLights.push_back(s);

		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position = glm::vec3(1075.f - i * 189.f, 0.f, -630.f);
		s->diffuseColor = glm::vec3(1.f, 1.f, .9f);
		s->diffuseIntensity = 2.f;
		s->specularIntensity = 1.f;
		s->specularPower = 32.f;
		s->attenuation = glm::vec3(.0001f, .00001f, .00001f);
		s->setPitch(-90.f);
		s->cutoffAngle = 25.f;
		spotLights.push_back(s);
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

void update(float delta)
{
	camera->update(input, delta);

	static float z = 0.f;
	pointLights[0]->position.x = sinf(z) * 1200.f;
	pointLights[1]->position.y = 500.f + sinf(z) * 500.f;
	z += .005f * delta;

	//manModel->setYaw(manModel->getYaw() + delta * .003f);
}

void render()
{
	assert(renderer);
	renderer->render(camera);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::stringstream s;
	unsigned int ms = (thisTickTime - lastTickTime);
	s << "frame time: " << ms << " ms     fps: " << 1000.f / ms;
	SDL_Color color = { 255, 255, 255 };
	fontRenderer->drawText(s.str(), 0.f, 0.f, color);

	billboardRenderer->render(camera);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	window->finalizeFrame();
}

void destroy()
{
	delete sceneManager;
	delete camera;
	delete renderer;
	delete fontRenderer;
	delete billboardRenderer;
	delete window;
}

int main(int argc, char **argv)
{
	if (!load())
		return -1;

	while (window->alive)
	{
		thisTickTime = SDL_GetTicks();
		float delta = (thisTickTime - lastTickTime) * .1f;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			// quit event
				window->alive = false;
			else if (event.type == SDL_MOUSEMOTION)
			// mouse input
			{
				int w2 = window->width / 2;
				int h2 = window->height / 2;

				camera->rotateYaw((event.motion.x - w2) / (float)w2);
				camera->rotatePitch((event.motion.y - h2) / (float)h2);

				SDL_WarpMouseInWindow(window->sdlWindow, w2, h2);
			}
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			// keyboard input
			{
				if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP)
					window->alive = false;
				else if (event.key.keysym.sym == SDLK_F10 && event.type == SDL_KEYUP)
					window->changeResolution(window->width, window->height, !window->fullscreen);
				else
					input.sendKeyboardEvent(event);
			}
		}

		update(delta);
		render();

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}