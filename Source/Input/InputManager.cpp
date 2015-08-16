#include "InputManager.hpp"
#include "..\Camera\Camera.hpp"
#include "..\UI\GBufferInspector.hpp"
#include "..\UI\LightEditor.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\ForwardRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern bool showBillboards;

extern Camera *camera;
extern GBufferInspector *gBufferInspector;
extern LightManager *lightManager;
extern LightEditor *lightEditor;
extern IRenderer *sceneRenderer;
extern Window *window;

InputManager::InputManager()
{
	forwardKeyPressed = backKeyPressed = leftKeyPressed = rightKeyPressed = upKeyPressed = downKeyPressed = crouchKeyPressed = false;

	lightForwardKeyPressed = lightBackKeyPressed = lightLeftKeyPressed = lightRightKeyPressed = lightUpKeyPressed = lightDownKeyPressed = false;

	debugMode = false;
	flashLight = false;
}

void InputManager::sendMouseButtonEvent(const SDL_Event &event)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (!camera->getFirstPerson())
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			lightEditor->onMouseButtonDown(glm::vec2(x, y), event.button.button);
			gBufferInspector->onMouseButtonDown(glm::vec2(x, y), event.button.button);
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			lightEditor->onMouseButtonUp(glm::vec2(x, y), event.button.button);
			gBufferInspector->onMouseButtonUp(glm::vec2(x, y), event.button.button);

			if (event.button.button == 1)
				lightManager->selectLight(glm::vec2(x, y));
		}
	}
}

void InputManager::sendMouseMoveEvent(const SDL_Event &event)
{
	lightEditor->onMouseMove(glm::vec2(event.motion.x, event.motion.y));
	gBufferInspector->onMouseMove(glm::vec2(event.motion.x, event.motion.y));

	if (camera->getFirstPerson())
	{
		int w2 = window->width / 2;
		int h2 = window->height / 2;

		camera->rotateYaw((event.motion.x - w2) / (float)w2);
		camera->rotatePitch((event.motion.y - h2) / (float)h2);

		SDL_WarpMouseInWindow(window->sdlWindow, w2, h2);
	}
}

void InputManager::sendMouseWheelEvent(const SDL_Event &event)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	lightEditor->onMouseWheel(glm::vec2(x, y), event.wheel.y);
	gBufferInspector->onMouseWheel(glm::vec2(x, y), event.wheel.y);
}

void InputManager::sendKeyboardEvent(const SDL_Event &event)
{
	if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP)
		window->alive = false;
	else if (event.key.keysym.sym == SDLK_F10 && event.type == SDL_KEYUP)
		window->changeResolution(window->width, window->height, !window->fullscreen);
	else if (event.key.keysym.sym == SDLK_TAB && event.type == SDL_KEYUP)
		debugMode = !debugMode;
	else if (event.key.keysym.sym == SDLK_f && event.type == SDL_KEYUP)
		flashLight = !flashLight;
	else if (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYUP)
		camera->setFirstPerson(!camera->getFirstPerson());
	else if (event.key.keysym.sym == SDLK_r && event.type == SDL_KEYUP)
	{
		assert(sceneRenderer);

		RendererType type = sceneRenderer->type;

		if (type == RendererType::DEFERRED_RENDERER)
			gBufferInspector->visible = false;

		delete sceneRenderer;

		if (type == RendererType::FORWARD_RENDERER)
			sceneRenderer = new DeferredRenderer();
		else if (type == RendererType::DEFERRED_RENDERER)
			sceneRenderer = new ForwardRenderer();

		sceneRenderer->init();
	}
	else if (event.key.keysym.sym == SDLK_x && event.type == SDL_KEYUP)
		showBillboards = !showBillboards;
	else if (event.key.keysym.sym == SDLK_g && event.type == SDL_KEYUP)
	{
		assert(sceneRenderer);
		if (sceneRenderer->type == RendererType::DEFERRED_RENDERER)
			gBufferInspector->visible = !gBufferInspector->visible;
	}
	else if (event.key.keysym.sym == SDLK_l && event.type == SDL_KEYUP)
		lightEditor->visible = !lightEditor->visible;

	else if (event.key.keysym.sym == SDLK_n && event.type == SDL_KEYUP)
	{
		if (sceneRenderer->type == RendererType::FORWARD_RENDERER)
			sceneRenderer->doAA = !sceneRenderer->doAA;
	}

	else if (event.key.keysym.sym == SDLK_w)
		forwardKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_s)
		backKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_a)
		leftKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_d)
		rightKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_e)
		upKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_q)
		downKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_LSHIFT)
		crouchKeyPressed = event.type == SDL_KEYDOWN;

	else if (event.key.keysym.sym == SDLK_u)
		lightForwardKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_j)
		lightBackKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_h)
		lightLeftKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_k)
		lightRightKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_i)
		lightUpKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_y)
		lightDownKeyPressed = event.type == SDL_KEYDOWN;
}