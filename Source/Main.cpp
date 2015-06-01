#include <sstream>

#include "Camera\Camera.hpp"
#include "Input\Input.hpp"
#include "Light\LightManager.hpp"
#include "Renderer\BillboardRenderer.hpp"
#include "Renderer\DeferredRenderer.hpp"
#include "Renderer\UIRenderer.hpp"
#include "Renderer\IRenderer.hpp"
#include "Renderer\UnitArrow.hpp"
#include "Renderer\UnitGizmo.hpp"
#include "Renderer\UnitQuad.hpp"
#include "Scene\SceneManager.hpp"
#include "UI\Label.hpp"
#include "UI\Frame.hpp"
#include "UI\Panel.hpp"
#include "UI\Slider.hpp"
#include "Util\Error.hpp"
#include "Util\Util.hpp"
#include "Window\Window.hpp"

BillboardRenderer *billboardRenderer;
Camera *camera;
UIRenderer *uiRenderer;
Frame *frame;
IRenderer *sceneRenderer;
Input input;
Label *sliderLabelRGB;
LightManager *lightManager;
Panel *panelDiffMap;
SceneManager *sceneManager;
Slider *sliderR, *sliderG, *sliderB;
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


	// load primitives

	if (!UnitQuad::create())
		return false;

	if (!UnitArrow::create())
		return false;

	if (!UnitGizmo::create())
		return false;
	

	// set up UI

	if (!Util::checkMemory(frame = new Frame(glm::vec2(window->width - 266.f, window->height - 266.f), glm::vec2(256.f, 256.f))))
		return false;

	if (!frame->load("LIGHT EDITOR"))
		return false;

	if (!Util::checkMemory(sliderR = new Slider(glm::vec2(30.f, 20.f), glm::vec2(196.f), 0, 255)))
		return false;

	if (!sliderR->load())
		return false;

	frame->addChildElement(sliderR);

	if (!Util::checkMemory(sliderG = new Slider(glm::vec2(30.f, 40.f), glm::vec2(196.f), 0, 255)))
		return false;

	if (!sliderG->load())
		return false;

	frame->addChildElement(sliderG);

	if (!Util::checkMemory(sliderB = new Slider(glm::vec2(30.f, 60.f), glm::vec2(196.f), 0, 255)))
		return false;

	if (!sliderB->load())
		return false;

	frame->addChildElement(sliderB);

	if (!Util::checkMemory(sliderLabelRGB = new Label(glm::vec2(30.f, 80.f))))
		return false;

	frame->addChildElement(sliderLabelRGB);

	if (!Util::checkMemory(panelDiffMap = new Panel(glm::vec2(48.f, 100.f), glm::vec2(160.f, 90.f))))
		return false;

	panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[1];

	frame->addChildElement(panelDiffMap);

	return true;
}

void update(float delta)
{
	assert(camera);
	camera->update(input, delta);

	assert(lightManager);
	lightManager->update(delta);

	glm::vec3 color(sliderR->value / 255.f, sliderG->value / 255.f, sliderB->value / 255.f);

	std::stringstream s;
	s << "Diffuse Color: " << sliderR->value << "/" << sliderG->value << "/" << sliderB->value;
	sliderLabelRGB->text = s.str();
	sliderLabelRGB->color = color;

	lightManager->spotLights[lightManager->getSelectedPointLight()]->diffuseColor = color;

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
	UnitArrow::destroy();
	UnitGizmo::destroy();

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