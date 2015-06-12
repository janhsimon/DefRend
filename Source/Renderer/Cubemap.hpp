#pragma once

#include <glew.h>

class Cubemap
{
public:
	GLuint handle;
	GLuint FBO;
	GLuint depthTexture;

	Cubemap();
	~Cubemap();
};