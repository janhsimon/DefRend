#include <sstream>

#include "Camera\Camera.hpp"
#include "Input\Input.hpp"
#include "Light\LightManager.hpp"
#include "Renderer\BillboardRenderer.hpp"
#include "Renderer\DeferredRenderer.hpp"
#include "Renderer\UIRenderer.hpp"
#include "Renderer\IRenderer.hpp"
#include "Renderer\UnitQuad.hpp"
#include "Scene\SceneManager.hpp"
#include "UI\Label.hpp"
#include "UI\Frame.hpp"
#include "Util\Error.hpp"
#include "Util\Util.hpp"
#include "Window\Window.hpp"

BillboardRenderer *billboardRenderer;
Camera *camera;
UIRenderer *uiRenderer;
Frame *frame;
IRenderer *sceneRenderer;
Input input;
LightManager *lightManager;
SceneManager *sceneManager;
Window *window;

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


	// create scene renderer

	if (!Util::checkMemory(sceneRenderer = new DeferredRenderer()))
		return false;

	if (!sceneRenderer->init(window))
		return false;


	// create font renderer

	if (!Util::checkMemory(uiRenderer = new UIRenderer()))
		return false;

	if (!uiRenderer->create())
		return false;


	// create camera

	if (!Util::checkMemory(camera = new Camera(glm::vec3(0.f, 100.f, 0.f))))
		return false;


	// create light manager

	if (!Util::checkMemory(lightManager = new LightManager()))
		return false;

	if (!lightManager->create())
		return false;
		

	// create scene manager

	if (!Util::checkMemory(sceneManager = new SceneManager()))
		return false;


	// load models

	if (!sceneManager->addModel("Models\\Sponza\\Sponza.obj"))
		return false;

	if (!sceneManager->addModel("Models\\OldMan\\OldMan.obj", glm::vec3(-500.f, -3.f, 0.f)))
		return false;


	// load unit quad

	if (!UnitQuad::create())
		return false;
	

	if (!Util::checkMemory(frame = new Frame(glm::vec2(window->width - 256.f, 0.f), glm::vec2(256.f, 256.f))))
		return false;

	if (!frame->load())
		return false;

	Label *label;

	if (!Util::checkMemory(label = new Label("Light Editor", glm::vec2(90.f, 15.f), glm::vec2(0.f, 0.f))))
		return false;

	frame->addChildElement(label);

	return true;
}

void update(float delta)
{
	assert(camera);
	camera->update(input, delta);

	assert(lightManager);
	lightManager->update(delta);

	//manModel->setYaw(manModel->getYaw() + delta * .003f);
}

void render()
{
	assert(sceneRenderer);
	sceneRenderer->render(camera);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frame->render(uiRenderer);

	std::stringstream s;
	unsigned int ms = (thisTickTime - lastTickTime);
	s << "FPS: " << int(1000.f / ms) << "        Frame Time: " << ms;
	SDL_Color color = { 255, 255, 255 };
	uiRenderer->drawText(s.str(), glm::vec2(5.f, window->height - 25.f), color);

	uiRenderer->drawText("SPACE to switch camera modes", glm::vec2(5.f, 5.f), color);

	billboardRenderer->render(camera);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	window->finalizeFrame();
}

void destroy()
{
	UnitQuad::destroy();

	delete frame;
	delete lightManager;
	delete sceneManager;
	delete camera;
	delete sceneRenderer;
	delete uiRenderer;
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
				window->alive = false;
			else if (event.type == SDL_MOUSEMOTION)
				input.sendMouseMoveEvent(event);
			else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
				input.sendMouseButtonEvent(event);
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
				input.sendKeyboardEvent(event);
		}

		update(delta);
		render();

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}