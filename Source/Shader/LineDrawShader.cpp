#include <gtc\type_ptr.hpp>

#include "LineDrawShader.hpp"
#include "..\Util\Error.hpp"

const std::string LineDrawShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string LineDrawShader::COLOR_UNIFORM_NAME = "color";

bool LineDrawShader::create()
{
	if (!load("Shaders\\LineDraw.vs.glsl", "", "Shaders\\LineDraw.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(program);

	if (!getUniformLocation(WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME, worldViewProjectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(COLOR_UNIFORM_NAME, colorUniformLocation))
		return false;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load line draw shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void LineDrawShader::setWorldViewProjectionMatrixUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldViewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr((projectionMatrix * viewMatrix) * worldMatrix));
}

void LineDrawShader::setColorUniform(const glm::vec3 &color)
{
	glUniform3f(colorUniformLocation, color.r, color.g, color.b);
}