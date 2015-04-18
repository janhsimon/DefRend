#pragma once

#include <SDL.h>

class Renderer
{
private:
	SDL_GLContext glContext;

public:
	~Renderer();

	bool create();
	void finalizeFrame();
	SDL_GLContext getGLContext();
};