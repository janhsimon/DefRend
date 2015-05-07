#pragma once

#include <SDL.h>

class Window
{
private:
	SDL_Window *sdlWindow;
	SDL_GLContext glContext;
	bool alive;
	unsigned int width, height;
	bool fullscreen;

public:
	~Window();

	bool create(unsigned int width, unsigned int height);
	bool changeResolution(unsigned int width, unsigned int height, bool fullscreen);
	void finalizeFrame();
	SDL_GLContext getGLContext();

	SDL_Window *getSDLWindow();
	bool getAlive();
	void setAlive(bool alive);
	unsigned int getWidth();
	unsigned int getHeight();
	bool getFullscreen();
};