#pragma once

#include <glew.h>

class UnitQuad
{
private:
	GLuint VBO;
	GLuint VAO;

public:
	~UnitQuad();

	bool create();
	void render();
};