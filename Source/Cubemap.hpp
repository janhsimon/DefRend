#pragma once

#include <glew.h>

class Cubemap
{
private:
	GLuint handle;
	GLuint FBO;

public:
	Cubemap();
	~Cubemap();

	GLuint getFBO();
};