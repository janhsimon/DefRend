#pragma once

#include <glew.h>

class UnitQuad
{
private:
	static GLuint VBO;
	static GLuint VAO;

public:
	static bool create();
	static void destroy();
	static void render();
};