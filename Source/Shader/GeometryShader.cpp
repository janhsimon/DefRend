#include <gtc/type_ptr.hpp>

#include "GeometryShader.hpp"
#include "..\Util\Error.hpp"

const std::string GeometryShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string GeometryShader::VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "viewProjectionMatrix";
const std::string GeometryShader::TEXTURE_MAP_UNIFORM_NAMES[4] = { "diffuseMap", "specularMap", "normalMap", "opacityMap" };

bool GeometryShader::create()
{
	if (!load("Shaders\\Geometry.vs.glsl", "", "Shaders\\Geometry.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(getProgram());

	if (!getUniformLocation(WORLD_MATRIX_UNIFORM_NAME, worldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(VIEW_PROJECTION_MATRIX_UNIFORM_NAME, viewProjectionMatrixUniformLocation))
		return false;

	GLint textureMapUniformLocation;

	for (int i = 0; i < 4; ++i)
	{
		if (!getUniformLocation(TEXTURE_MAP_UNIFORM_NAMES[i], textureMapUniformLocation))
			return false;

		glUniform1i(textureMapUniformLocation, i);
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load geometry shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void GeometryShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix));
}