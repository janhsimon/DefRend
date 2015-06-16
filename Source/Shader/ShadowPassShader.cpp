#include <gtc\type_ptr.hpp>

#include "ShadowPassShader.hpp"
#include "..\Util\Error.hpp"

const std::string ShadowPassShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string ShadowPassShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string ShadowPassShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
const std::string ShadowPassShader::OPACITY_MAP_UNIFORM_NAME = "opacityMap";
const std::string ShadowPassShader::POINT_LIGHT_POSITION_UNIFORM_NAME = "pointLightPosition";
const std::string ShadowPassShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "cameraFarClip";

bool ShadowPassShader::create()
{
	if (!load("Shaders\\ShadowPass.vs.glsl", "", "Shaders\\ShadowPass.fs.glsl"))
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

	GLint opacityMapUniformLocation;

	if (!getUniformLocation(OPACITY_MAP_UNIFORM_NAME, opacityMapUniformLocation))
			return false;

	glUniform1i(opacityMapUniformLocation, 0);

	if (!getUniformLocation(POINT_LIGHT_POSITION_UNIFORM_NAME, pointLightPositionUniformLocation))
		return false;

	if (!getUniformLocation(CAMERA_FAR_CLIP_UNIFORM_NAME, cameraFarClipUniformLocation))
		return false;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load shadow pass shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void ShadowPassShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void ShadowPassShader::setPointLightPositionUniform(const glm::vec3 &pointLightPosition)
{
	glUniform3f(pointLightPositionUniformLocation, pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
}

void ShadowPassShader::setCameraFarClipUniform(float cameraFarClip)
{
	glUniform1f(cameraFarClipUniformLocation, cameraFarClip);
}