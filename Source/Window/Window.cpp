#include <assert.h>

#include "..\Util\Error.hpp"
#include "Window.hpp"

Window::~Window()
{
	if (glContext)
		SDL_GL_DeleteContext(glContext);

	if (sdlWindow)
		SDL_DestroyWindow(sdlWindow);

	SDL_Quit();
}

bool Window::create(unsigned int width, unsigned int height)
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// use opengl version 3.3 (from 4.4)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Error::report("Error", "SDL_Init Error: " + std::string(SDL_GetError()));
		return false;
	}

	sdlWindow = SDL_CreateWindow("DefRend -- written by Jan Simon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (!sdlWindow)
	{
		Error::report("Error", "SDL_CreateWindow Error" + std::string(SDL_GetError()));
		return false;
	}

	// create an opengl context for the window
	glContext = SDL_GL_CreateContext(sdlWindow);

	if (glContext == NULL)
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

	// enable culling
	glEnable(GL_CULL_FACE);

	alive = true;
	this->width = width;
	this->height = height;
	fullscreen = false;

	return true;
}

bool Window::changeResolution(unsigned int width, unsigned int height, bool fullscreen)
{
	/*
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		Error::report("Error", "SDL_GetDesktopDisplayMode Error: " + std::string(SDL_GetError()));
		return false;
	}

	if (!fullscreen)
		SDL_SetWindowSize(sdlWindow, width, height);
	else
		SDL_SetWindowSize(sdlWindow, dm.w, dm.h);
	*/

	if (SDL_SetWindowFullscreen(sdlWindow, fullscreen ? /*SDL_WINDOW_FULLSCREEN_DESKTOP*/SDL_WINDOW_FULLSCREEN : 0) < 0)
	{
		Error::report("Error", "SDL_SetWindowFullscreen Error: " + std::string(SDL_GetError()));
		return false;
	}

	this->width = width;
	this->height = height;
	this->fullscreen = fullscreen;

	return true;
}

void Window::finalizeFrame()
{
	assert(sdlWindow);

	// swap the buffers
	SDL_GL_SwapWindow(sdlWindow);
}