#pragma once

#include <glew.h>

class Cubemap
{
public:
	GLuint handle;
	GLuint FBO;
	GLuint depthTexture;

	unsigned int size;

	Cubemap(unsigned int size);
	~Cubemap();
};