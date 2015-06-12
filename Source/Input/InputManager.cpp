#include "InputManager.hpp"
#include "..\Camera\Camera.hpp"
#include "..\UI\GBufferInspector.hpp"
#include "..\UI\LightEditor.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Window\Window.hpp"

extern Camera *camera;
extern GBufferInspector *gBufferInspector;
extern LightManager *lightManager;
extern LightEditor *lightEditor;
extern Window *window;

InputManager::InputManager()
{
	forwardKeyPressed = backKeyPressed = leftKeyPressed = rightKeyPressed = crouchKeyPressed = false;

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
				lightManager->selectPointLight(glm::vec2(x, y));
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

	else if (event.key.keysym.sym == SDLK_w)
		forwardKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_s)
		backKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_a)
		leftKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_d)
		rightKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_LSHIFT)
		crouchKeyPressed = event.type == SDL_KEYDOWN;

	else if (event.key.keysym.sym == SDLK_i)
		lightForwardKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_k)
		lightBackKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_j)
		lightLeftKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_l)
		lightRightKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_u)
		lightDownKeyPressed = event.type == SDL_KEYDOWN;
	else if (event.key.keysym.sym == SDLK_o)
		lightUpKeyPressed = event.type == SDL_KEYDOWN;
}