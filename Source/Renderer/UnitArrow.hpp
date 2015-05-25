#pragma once

#include <glew.h>

class UnitArrow
{
private:
	GLuint VBO;
	GLuint VAO;

public:
	~UnitArrow();

	bool create();
	void render();
};