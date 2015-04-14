#include "Error.hpp"
#include "ShaderProgram.hpp"

#include <assert.h>
#include <fstream>
#include <sstream>

bool ShaderProgram::loadShader(GLuint shader, const std::string &filename)
{
	assert(shader != 0);

	std::ifstream file(filename);

	if (!file)
	{
		Error::report("Error", "Failed to open shader file \"" + filename + "\".");
		return false;
	}

	std::stringstream source;
	source << file.rdbuf();

	const std::string &tmp = source.str();
	const char *c_str = tmp.c_str();

	glShaderSource(shader, 1, &c_str, nullptr);
	compileShader(shader, filename);
	glAttachShader(program, shader);

	return true;
}

bool ShaderProgram::compileShader(GLuint shader, const std::string &filename)
{
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);

		Error::report("Failed to compile shader \"" + filename + "\".", log);

		delete[] log;

		return false;
	}

	return true;
}

bool ShaderProgram::load(const std::string &vertexShaderFilename, const std::string &geometryShaderFilename, const std::string &fragmentShaderFilename)
{
	program = glCreateProgram();

	if (!program)
	{
		Error::report("Error", "Failed to create shader program.");
		return false;
	}
	
	if (!vertexShaderFilename.empty())
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		if (!loadShader(vertexShader, vertexShaderFilename))
			return false;
	}

	if (!geometryShaderFilename.empty())
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		if (!loadShader(geometryShader, geometryShaderFilename))
			return false;
	}

	if (!fragmentShaderFilename.empty())
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		if (!loadShader(fragmentShader, fragmentShaderFilename))
			return false;
	}

	return true;
}

bool ShaderProgram::link()
{
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);

		Error::report("Failed to link shader program.", log);

		delete[] log;

		return false;
	}

	return true;
}

bool ShaderProgram::getUniformLocation(const std::string &uniformName, GLint &locationOut)
{
	locationOut = glGetUniformLocation(program, uniformName.c_str());

	if (locationOut < 0)
	{
		Error::report("Error", "Failed to get location of uniform \"" + uniformName + "\".");
		return false;
	}

	return true;
}

ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

GLuint ShaderProgram::getProgram()
{
	return program;
}

GLuint ShaderProgram::getVertexShader()
{
	return vertexShader;
}

GLuint ShaderProgram::getGeometryShader()
{
	return geometryShader;
}

GLuint ShaderProgram::getFragmentShader()
{
	return fragmentShader;
}
