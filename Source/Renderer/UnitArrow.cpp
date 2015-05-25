#include "UnitArrow.hpp"
#include "..\Util\Error.hpp"

UnitArrow::~UnitArrow()
{

}

bool UnitArrow::create()
{
	float data[10][3] =
	{
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 1.f },

		{ 0.f, 0.f, 1.f },
		{ -.1f, 0.f, .9f },

		{ 0.f, 0.f, 1.f },
		{ .1f, 0.f, .9f },
	};

	// generate and bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// enable vertex attribute array
	glEnableVertexAttribArray(0);

	// generate and bind a VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// set vertex attribute array pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // position

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate VBO and VAO for unit arrow: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void UnitArrow::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glLineWidth(3.f);
	glDrawArrays(GL_LINES, 0, 6);
}