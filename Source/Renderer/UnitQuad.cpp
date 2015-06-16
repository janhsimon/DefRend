#include "UnitQuad.hpp"
#include "..\Util\Error.hpp"

GLuint UnitQuad::VBO;
GLuint UnitQuad::VAO;

bool UnitQuad::create()
{
	float data[4][4] =
	{
		{ -1.f, 1.f, 0.f, 0.f },
		{ 1.f, 1.f, 1.f, 0.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, -1.f, 1.f, 1.f }
	};

	// generate and bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// generate and bind a VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// set vertex attribute array pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);								// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));	// tex coord uvs

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate VBO and VAO for unit quad: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void UnitQuad::destroy()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void UnitQuad::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}