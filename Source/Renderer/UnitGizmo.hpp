#pragma once

#include <glew.h>

class UnitGizmo
{
private:
	static const float LINE_LENGTH;

	static GLuint VBO;
	static GLuint VAO;
	

public:
	static bool create();
	static void destroy();
	static void render();
};