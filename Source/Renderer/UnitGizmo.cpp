#include "UnitGizmo.hpp"
#include "..\Util\Error.hpp"

const float UnitGizmo::LINE_LENGTH = .55f;

GLuint UnitGizmo::VBO;
GLuint UnitGizmo::VAO;

bool UnitGizmo::create()
{
	float data[48][3] =
	{
		{ -1.f, -1.f, -1.f },
		{ -1.f + LINE_LENGTH, -1.f, -1.f },

		{ -1.f, -1.f, -1.f },
		{ -1.f, -1.f + LINE_LENGTH, -1.f },

		{ -1.f, -1.f, -1.f },
		{ -1.f, -1.f, -1.f + LINE_LENGTH },


		{ -1.f, -1.f, 1.f },
		{ -1.f + LINE_LENGTH, -1.f, 1.f },

		{ -1.f, -1.f, 1.f },
		{ -1.f, -1.f + LINE_LENGTH, 1.f },

		{ -1.f, -1.f, 1.f },
		{ -1.f, -1.f, 1.f - LINE_LENGTH },


		{ -1.f, 1.f, 1.f },
		{ -1.f + LINE_LENGTH, 1.f, 1.f },

		{ -1.f, 1.f, 1.f },
		{ -1.f, 1.f - LINE_LENGTH, 1.f },

		{ -1.f, 1.f, 1.f },
		{ -1.f, 1.f, 1.f - LINE_LENGTH },


		{ -1.f, 1.f, -1.f },
		{ -1.f + LINE_LENGTH, 1.f, -1.f },

		{ -1.f, 1.f, -1.f },
		{ -1.f, 1.f - LINE_LENGTH, -1.f },

		{ -1.f, 1.f, -1.f },
		{ -1.f, 1.f, -1.f + LINE_LENGTH },


		{ 1.f, 1.f, 1.f },
		{ 1.f - LINE_LENGTH, 1.f, 1.f },

		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f - LINE_LENGTH, 1.f },

		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f - LINE_LENGTH },


		{ 1.f, -1.f, -1.f },
		{ 1.f - LINE_LENGTH, -1.f, -1.f },

		{ 1.f, -1.f, -1.f },
		{ 1.f, -1.f + LINE_LENGTH, -1.f },

		{ 1.f, -1.f, -1.f },
		{ 1.f, -1.f, -1.f + LINE_LENGTH },


		{ 1.f, 1.f, -1.f },
		{ 1.f - LINE_LENGTH, 1.f, -1.f },

		{ 1.f, 1.f, -1.f },
		{ 1.f, 1.f - LINE_LENGTH, -1.f },

		{ 1.f, 1.f, -1.f },
		{ 1.f, 1.f, -1.f + LINE_LENGTH },


		{ 1.f, -1.f, 1.f },
		{ 1.f - LINE_LENGTH, -1.f, 1.f },

		{ 1.f, -1.f, 1.f },
		{ 1.f, -1.f + LINE_LENGTH, 1.f },

		{ 1.f, -1.f, 1.f },
		{ 1.f, -1.f, 1.f - LINE_LENGTH }
		
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
		Error::report("Error", "Failed to generate VBO and VAO for unit gizmo: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void UnitGizmo::destroy()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void UnitGizmo::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glLineWidth(1.f);
	glDrawArrays(GL_LINES, 0, 48);
}