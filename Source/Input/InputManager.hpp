#pragma once

#include <SDL.h>

class InputManager
{
public:
	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool crouchKeyPressed;

	bool debugMode;
	bool flashLight;

	InputManager();

	void sendMouseButtonEvent(const SDL_Event &event);
	void sendMouseMoveEvent(const SDL_Event &event);
	void sendKeyboardEvent(const SDL_Event &event);
};