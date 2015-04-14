#include "Error.hpp"
#include "Window.hpp"

Window::~Window()
{
	if (sdlWindow)
		SDL_DestroyWindow(sdlWindow);

	SDL_Quit();
}

bool Window::create(unsigned int width, unsigned int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Error::report("Error", "SDL_Init Error: " + std::string(SDL_GetError()));
		return false;
	}

	sdlWindow = SDL_CreateWindow("DefRend -- written by Jan Simon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!sdlWindow)
	{
		Error::report("Error", "SDL_CreateWindow Error" + std::string(SDL_GetError()));
		return false;
	}

	// hide the mouse cursor
	SDL_ShowCursor(false);

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

	if (SDL_SetWindowFullscreen(sdlWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
	{
		Error::report("Error", "SDL_SetWindowFullscreen Error: " + std::string(SDL_GetError()));
		return false;
	}

	this->width = width;
	this->height = height;
	this->fullscreen = fullscreen;

	return true;
}

SDL_Window *Window::getSDLWindow()
{
	return sdlWindow;
}

bool Window::getAlive()
{
	return alive;
}

void Window::setAlive(bool alive)
{
	this->alive = alive;
}

unsigned int Window::getWidth()
{
	return width;
}

unsigned int Window::getHeight()
{
	return height;
}

bool Window::getFullscreen()
{
	return fullscreen;
}