#pragma once

#include <SDL.h>

class Renderer
{
private:
	SDL_GLContext glContext;

public:
	~Renderer();

	bool create();
	void beginDrawing();
	void endDrawing();
	SDL_GLContext getGLContext();
};