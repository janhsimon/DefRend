#pragma once

#include "Camera.hpp"
#include "Window.hpp"

class IRenderer
{
protected:
	Window *window;

public:
	virtual ~IRenderer() {};

	virtual bool init(Window *window) = 0;
	virtual void render(Camera *camera) = 0;
};