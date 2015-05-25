#pragma once

#include <SDL.h>

class Input
{
public:
	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool crouchKeyPressed;

	bool debugMode;
	bool flashLight;

	Input();

	void sendKeyboardEvent(const SDL_Event &event);
};