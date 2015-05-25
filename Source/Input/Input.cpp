#include "Input.hpp"

Input::Input()
{
	forwardKeyPressed = backKeyPressed = leftKeyPressed = rightKeyPressed = crouchKeyPressed = false;

	debugMode = false;
	flashLight = false;
}

void Input::sendKeyboardEvent(const SDL_Event &event)
{
	if (event.key.keysym.sym == SDLK_TAB && event.type == SDL_KEYUP)
		debugMode = !debugMode;
	else if (event.key.keysym.sym == SDLK_f && event.type == SDL_KEYUP)
		flashLight = !flashLight;
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