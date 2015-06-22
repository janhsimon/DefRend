#pragma once

#include <SDL.h>

class InputManager
{
public:
	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool upKeyPressed;
	bool downKeyPressed;
	bool crouchKeyPressed;

	bool lightForwardKeyPressed;
	bool lightBackKeyPressed;
	bool lightLeftKeyPressed;
	bool lightRightKeyPressed;
	bool lightUpKeyPressed;
	bool lightDownKeyPressed;

	bool debugMode;
	bool flashLight;

	InputManager();

	void sendMouseButtonEvent(const SDL_Event &event);
	void sendMouseMoveEvent(const SDL_Event &event);
	void sendMouseWheelEvent(const SDL_Event &event);
	void sendKeyboardEvent(const SDL_Event &event);
};