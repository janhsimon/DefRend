#include <sstream>

#include "Camera\Camera.hpp"
#include "Input\InputManager.hpp"
#include "Light\LightManager.hpp"
#include "Renderer\BillboardRenderer.hpp"
#include "Renderer\DeferredRenderer.hpp"
#include "Renderer\UIRenderer.hpp"
#include "Renderer\IRenderer.hpp"
#include "Renderer\UnitArrow.hpp"
#include "Renderer\UnitGizmo.hpp"
#include "Renderer\UnitQuad.hpp"
#include "Scene\SceneManager.hpp"
#include "UI\GBufferInspector.hpp"
#include "UI\LightEditor.hpp"
#include "Util\Error.hpp"
#include "Util\Util.hpp"
#include "Window\Window.hpp"

BillboardRenderer *billboardRenderer;
Camera *camera;
UIRenderer *uiRenderer;
GBufferInspector *gBufferInspector;
IRenderer *sceneRenderer;
InputManager *inputManager;
LightEditor *lightEditor;
LightManager *lightManager;
SceneManager *sceneManager;
Window *window;

unsigned int thisTickTime, lastTickTime = 0;
glm::mat4 projectionMatrix;
bool showBillboards = false;

bool load()
{
	// create a window

	if (!Util::checkMemory(window = new Window()))
		return false;

	if (!window->create(1280, 720))
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

	//if (!GLEW_VERSION_4_4)
	if (!GLEW_VERSION_3_0)
	{
		Error::report("Error", "OpenGL v3.0 is required but your system does not support it.");
		return false;
	}

	error = glGetError();
	if (error != GL_NO_ERROR && error != GL_INVALID_ENUM)
	// ignore a possible invalid enum error after GLEW initialization, known OpenGL issue without fix :(
	{
		Error::report("Error", "Failed to initialize GLEW: " + Error::getOpenGLErrorString(error));
		return false;
	}


	// create billboard, scene and UI renderers

	if (!Util::checkMemory(billboardRenderer = new BillboardRenderer()))
		return false;

	if (!billboardRenderer->create())
		return false;

	if (!Util::checkMemory(sceneRenderer = new DeferredRenderer()))
		return false;

	if (!sceneRenderer->init())
		return false;

	if (!Util::checkMemory(uiRenderer = new UIRenderer()))
		return false;

	if (!uiRenderer->create())
		return false;


	// create camera

	if (!Util::checkMemory(camera = new Camera(glm::vec3(0.f, 100.f, 0.f))))
		return false;


	// create light, scene and input managers

	if (!Util::checkMemory(lightManager = new LightManager()))
		return false;

	if (!lightManager->create())
		return false;
		
	if (!Util::checkMemory(sceneManager = new SceneManager()))
		return false;

	if (!Util::checkMemory(inputManager = new InputManager()))
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
	

	// load UI elements

	if (!Util::checkMemory(lightEditor = new LightEditor(glm::vec2(window->width - 266.f, window->height - 522.f))))
		return false;

	if (!lightEditor->create())
		return false;

	if (!Util::checkMemory(gBufferInspector = new GBufferInspector(glm::vec2(window->width - 420.f, 48.f))))
		return false;

	if (!gBufferInspector->create())
		return false;


	return true;
}

void update(float delta)
{
	assert(camera);
	camera->update(delta);

	assert(lightManager);
	lightManager->update(delta);

	assert(lightEditor);
	lightEditor->update();

	assert(gBufferInspector);
	gBufferInspector->update();

	sceneManager->models[1]->setYaw(sceneManager->models[1]->getYaw() + delta * .3f);
}

void render()
{
	assert(sceneRenderer);
	sceneRenderer->render(camera);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (showBillboards)
		billboardRenderer->render(camera);

	lightEditor->render();
	gBufferInspector->render();

	std::stringstream s;
	unsigned int ms = (thisTickTime - lastTickTime);
	s << "FPS: " << int(1000.f / ms) << "        Frame Time: " << ms << " ms        Light Count: " << lightManager->lights.size();
	SDL_Color color = { 255, 255, 255 };
	uiRenderer->drawText(s.str(), glm::vec2(5.f, window->height - 25.f), color);

	uiRenderer->drawText("[W], [A], [S], [D], [Q], [E] to MOVE the camera", glm::vec2(5.f, 5.f), color);

	uiRenderer->drawText("[U], [H], [J], [K], [Y], [I] to MOVE the selected light", glm::vec2(5.f, 23.f), color);

	uiRenderer->drawText("[SHIFT] to SLOW camera movement", glm::vec2(5.f, 41.f), color);

	if (camera->getFirstPerson())
		uiRenderer->drawText("[SPACE] to UNLOCK mouse cursor", glm::vec2(5.f, 59.f), color);
	else
		uiRenderer->drawText("[SPACE] to LOCK mouse cursor", glm::vec2(5.f, 59.f), color);
	
	if (sceneRenderer->type == RendererType::FORWARD_RENDERER)
		uiRenderer->drawText("[R] to SWITCH to Deferred Renderer", glm::vec2(5.f, 77.f), color);
	else
		uiRenderer->drawText("[R] to SWITCH to Forward Renderer", glm::vec2(5.f, 77.f), color);

	if (showBillboards)
		uiRenderer->drawText("[X] to HIDE light icons", glm::vec2(5.f, 95.f), color);
	else
		uiRenderer->drawText("[X] to SHOW light icons", glm::vec2(5.f, 95.f), color);

	if (gBufferInspector->visible)
		uiRenderer->drawText("[G] to HIDE \"G-Buffer Inspector\" window", glm::vec2(5.f, 112.f), color);
	else
		uiRenderer->drawText("[G] to SHOW \"G-Buffer Inspector\" window", glm::vec2(5.f, 112.f), color);

	if (lightEditor->visible)
		uiRenderer->drawText("[L] to HIDE \"Light Editor\" window", glm::vec2(5.f, 130.f), color);
	else
		uiRenderer->drawText("[L] to SHOW \"Light Editor\" window", glm::vec2(5.f, 130.f), color);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	window->finalizeFrame();
}

void destroy()
{
	UnitQuad::destroy();
	UnitArrow::destroy();
	UnitGizmo::destroy();

	delete lightEditor;
	delete gBufferInspector;
	delete lightManager;
	delete sceneManager;
	delete inputManager;
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
		
		// TODO: move down just over render call
		update(delta);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				window->alive = false;
			else if (event.type == SDL_MOUSEMOTION)
				inputManager->sendMouseMoveEvent(event);
			else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
				inputManager->sendMouseButtonEvent(event);
			else if (event.type == SDL_MOUSEWHEEL)
				inputManager->sendMouseWheelEvent(event);
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
				inputManager->sendKeyboardEvent(event);
		}
		
		render();

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}