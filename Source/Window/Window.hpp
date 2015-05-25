#pragma once

#include <SDL.h>

class Window
{
public:
	SDL_Window *sdlWindow;
	SDL_GLContext glContext;
	bool alive;
	unsigned int width, height;
	bool fullscreen;

	~Window();

	bool create(unsigned int width, unsigned int height);
	bool changeResolution(unsigned int width, unsigned int height, bool fullscreen);
	void finalizeFrame();
};