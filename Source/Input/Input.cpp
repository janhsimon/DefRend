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
		setDebugMode(!isDebugModeOn());
	else if (event.key.keysym.sym == SDLK_f && event.type == SDL_KEYUP)
		setFlashLight(!isFlashLightOn());
	else if (event.key.keysym.sym == SDLK_w)
		setForwardKeyPressed(event.type == SDL_KEYDOWN);
	else if (event.key.keysym.sym == SDLK_s)
		setBackKeyPressed(event.type == SDL_KEYDOWN);
	else if (event.key.keysym.sym == SDLK_a)
		setLeftKeyPressed(event.type == SDL_KEYDOWN);
	else if (event.key.keysym.sym == SDLK_d)
		setRightKeyPressed(event.type == SDL_KEYDOWN);
	else if (event.key.keysym.sym == SDLK_LSHIFT)
		setCrouchKeyPressed(event.type == SDL_KEYDOWN);
}

bool Input::isForwardKeyPressed()
{
	return forwardKeyPressed;
}

void Input::setForwardKeyPressed(bool pressed)
{
	forwardKeyPressed = pressed;
}

bool Input::isBackKeyPressed()
{
	return backKeyPressed;
}

void Input::setBackKeyPressed(bool pressed)
{
	backKeyPressed = pressed;
}

bool Input::isLeftKeyPressed()
{
	return leftKeyPressed;
}

void Input::setLeftKeyPressed(bool pressed)
{
	leftKeyPressed = pressed;
}

bool Input::isRightKeyPressed()
{
	return rightKeyPressed;
}

void Input::setRightKeyPressed(bool pressed)
{
	rightKeyPressed = pressed;
}

bool Input::isCrouchKeyPressed()
{
	return crouchKeyPressed;
}

void Input::setCrouchKeyPressed(bool pressed)
{
	crouchKeyPressed = pressed;
}

bool Input::isDebugModeOn()
{
	return debugMode;
}

void Input::setDebugMode(bool toggle)
{
	debugMode = toggle;
}

bool Input::isFlashLightOn()
{
	return flashLight;
}

void Input::setFlashLight(bool toggle)
{
	flashLight = toggle;
}
