#pragma once

#include "..\Camera\Camera.hpp"

enum RendererType
{
	FORWARD_RENDERER,
	DEFERRED_RENDERER
};

class IRenderer
{
public:
	RendererType type;

	virtual ~IRenderer() {};

	virtual bool init() = 0;
	virtual void render(Camera *camera) = 0;
};