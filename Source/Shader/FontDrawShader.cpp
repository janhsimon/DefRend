#include <gtc\type_ptr.hpp>

#include "FontDrawShader.hpp"
#include "..\Util\Error.hpp"

const std::string FontDrawShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string FontDrawShader::DIFFUSE_MAP_UNIFORM_NAME = "diffuseMap";

bool FontDrawShader::create()
{
	if (!load("Shaders\\FontDraw.vs.glsl", "", "Shaders\\FontDraw.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(getProgram());

	if (!getUniformLocation(WORLD_MATRIX_UNIFORM_NAME, worldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(DIFFUSE_MAP_UNIFORM_NAME, diffuseMapUniformLocation))
		return false;

	glUniform1i(diffuseMapUniformLocation, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load font draw shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void FontDrawShader::setWorldMatrixUniform(const glm::mat4 &worldMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
}