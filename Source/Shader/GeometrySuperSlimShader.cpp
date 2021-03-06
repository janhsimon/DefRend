#include <gtc\type_ptr.hpp>

#include "GeometrySuperSlimShader.hpp"
#include "..\Util\Error.hpp"

const std::string GeometrySuperSlimShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string GeometrySuperSlimShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string GeometrySuperSlimShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
const std::string GeometrySuperSlimShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "cameraFarClip";
const std::string GeometrySuperSlimShader::TEXTURE_MAP_UNIFORM_NAMES[4] = { "diffuseMap", "specularMap", "normalMap", "opacityMap" };

bool GeometrySuperSlimShader::create()
{
	if (!load("Shaders\\Geometry.vs.glsl", "", "Shaders\\GeometrySuperSlim.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(program);

	if (!getUniformLocation(WORLD_MATRIX_UNIFORM_NAME, worldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(VIEW_MATRIX_UNIFORM_NAME, viewMatrixUniformLocation))
		return false;

	if (!getUniformLocation(PROJECTION_MATRIX_UNIFORM_NAME, projectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(CAMERA_FAR_CLIP_UNIFORM_NAME, cameraFarClipUniformLocation))
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
		Error::report("Error", "Failed to load super slim geometry shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void GeometrySuperSlimShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void GeometrySuperSlimShader::setCameraFarClip(float cameraFarClip)
{
	glUniform1f(cameraFarClipUniformLocation, cameraFarClip);
}