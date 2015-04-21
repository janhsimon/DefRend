#include "Error.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

#include <assert.h>
#include <SDL_opengl.h>

extern Window *window;

Renderer::~Renderer()
{
	if (glContext)
		SDL_GL_DeleteContext(glContext);
}

bool Renderer::create()
{
	assert(window);
	assert(window->getSDLWindow());

	// use opengl version 4.4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	// require hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// request a 24-bit depth buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// create an opengl context for the window
	glContext = SDL_GL_CreateContext(window->getSDLWindow());

	if (!glContext)
	{
		Error::report("Error", "SDL_GL_CreateContext Error: " + std::string(SDL_GetError()));
		return false;
	}

	if (SDL_GL_SetSwapInterval(0) != 0)
	// disable vsync
	{
		Error::report("Error", "SDL_GL_SetSwapInterval Error: " + std::string(SDL_GetError()));
		return false;
	}

	// set clear color
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// set up blend func for opacity maps
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Renderer::finalizeFrame()
{
	assert(window);
	assert(window->getSDLWindow());

	// swap the buffers
	SDL_GL_SwapWindow(window->getSDLWindow());
}

SDL_GLContext Renderer::getGLContext()
{
	return glContext;
}