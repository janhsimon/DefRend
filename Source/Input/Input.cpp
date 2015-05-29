#include "Input.hpp"
#include "..\Camera\Camera.hpp"
#include "..\UI\Frame.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Window\Window.hpp"

extern Camera *camera;
extern LightManager *lightManager;
extern Frame *frame;
extern Window *window;

Input::Input()
{
	forwardKeyPressed = backKeyPressed = leftKeyPressed = rightKeyPressed = crouchKeyPressed = false;

	debugMode = false;
	flashLight = false;
}

void Input::sendMouseButtonEvent(const SDL_Event &event)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (!camera->getFirstPerson())
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
			frame->onMouseButtonDown(glm::vec2(x, y), event.button.button);
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			frame->onMouseButtonUp(glm::vec2(x, y), event.button.button);

			if (event.button.button == 1)
				lightManager->selectPointLight(glm::vec2(x, y));
		}
	}
}

void Input::sendMouseMoveEvent(const SDL_Event &event)
{
	frame->onMouseMove(glm::vec2(event.motion.x, event.motion.y));

	if (camera->getFirstPerson())
	{
		int w2 = window->width / 2;
		int h2 = window->height / 2;

		camera->rotateYaw((event.motion.x - w2) / (float)w2);
		camera->rotatePitch((event.motion.y - h2) / (float)h2);

		SDL_WarpMouseInWindow(window->sdlWindow, w2, h2);
	}
}

void Input::sendKeyboardEvent(const SDL_Event &event)
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
}