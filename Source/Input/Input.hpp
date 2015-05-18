#pragma once

#include <SDL.h>

class Input
{
private:
	bool forwardKeyPressed;
	bool backKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;
	bool crouchKeyPressed;

	bool debugMode;
	bool flashLight;

public:
	Input();

	void sendKeyboardEvent(const SDL_Event &event);

	bool isForwardKeyPressed();
	void setForwardKeyPressed(bool pressed);

	bool isBackKeyPressed();
	void setBackKeyPressed(bool pressed);

	bool isLeftKeyPressed();
	void setLeftKeyPressed(bool pressed);

	bool isRightKeyPressed();
	void setRightKeyPressed(bool pressed);

	bool isCrouchKeyPressed();
	void setCrouchKeyPressed(bool pressed);

	bool isDebugModeOn();
	void setDebugMode(bool toggle);

	bool isFlashLightOn();
	void setFlashLight(bool toggle);
	
};