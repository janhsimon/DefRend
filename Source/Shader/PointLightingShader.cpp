#include <gtc\type_ptr.hpp>

#include "PointLightingShader.hpp"
#include "..\Util\Error.hpp"

const std::string PointLightingShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string PointLightingShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string PointLightingShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
//const std::string PointLightingShader::INV_WORLD_MATRIX_UNIFORM_NAME = "invWorldMatrix";
//const std::string PointLightingShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "cameraFarClip";
const std::string PointLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string PointLightingShader::LIGHT_POSITION_UNIFORM_NAME = "lightPosition";
const std::string PointLightingShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "lightDiffuseColor";
const std::string PointLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string PointLightingShader::LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME = "lightSpecularIntensity";
const std::string PointLightingShader::LIGHT_SPECULAR_POWER_UNIFORM_NAME = "lightSpecularPower";
//const std::string PointLightingShader::LIGHT_ATTENUATION_UNIFORM_NAME = "lightAttenuation";
const std::string PointLightingShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";
const std::string PointLightingShader::SHADOW_BIAS_UNIFORM_NAME = "shadowBias";
const std::string PointLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[2] = { "inGBufferMRT0", "inGBufferMRT1" };
const std::string PointLightingShader::SHADOW_MAP_UNIFORM_NAME = "inShadowMap";

bool PointLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\PointLighting.fs.glsl"))
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

	/*
	if (!getUniformLocation(INV_WORLD_MATRIX_UNIFORM_NAME, invWorldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(CAMERA_FAR_CLIP_UNIFORM_NAME, cameraFarClipUniformLocation))
		return false;
	*/

	if (!getUniformLocation(SCREEN_SIZE_UNIFORM_NAME, screenSizeUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_POSITION_UNIFORM_NAME, lightPositionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_COLOR_UNIFORM_NAME, lightDiffuseColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME, lightDiffuseIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME, lightSpecularIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_POWER_UNIFORM_NAME, lightSpecularPowerUniformLocation))
		return false;

	//if (!getUniformLocation(LIGHT_ATTENUATION_UNIFORM_NAME, lightAttenuationUniformLocation))
		//return false;

	if (!getUniformLocation(EYE_POSITION_UNIFORM_NAME, eyePositionUniformLocation))
		return false;

	if (!getUniformLocation(SHADOW_BIAS_UNIFORM_NAME, shadowBiasUniformLocation))
		return false;

	GLint gBufferMapsUniformLocation;

	for (int i = 0; i < 2; ++i)
	{
		if (!getUniformLocation(GBUFFER_MAPS_UNIFORM_NAMES[i], gBufferMapsUniformLocation))
			return false;

		glUniform1i(gBufferMapsUniformLocation, i);
	}

	GLint shadowMapUniformLocation;

	if (!getUniformLocation(SHADOW_MAP_UNIFORM_NAME, shadowMapUniformLocation))
		return false;

	glUniform1i(shadowMapUniformLocation, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load point lighting shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

/*
void PointLightingShader::setCameraFarClipUniform(float cameraFarClip)
{
	glUniform1f(cameraFarClipUniformLocation, cameraFarClip);
}
*/

void PointLightingShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//glUniformMatrix4fv(invWorldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::inverse(viewMatrix)));
}

void PointLightingShader::setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void PointLightingShader::setLightParameters(const PointLight *pointLight)
{
	assert(pointLight);

	glUniform3f(lightPositionUniformLocation, pointLight->position.x, pointLight->position.y, pointLight->position.z);
	glUniform3f(lightDiffuseColorUniformLocation, pointLight->diffuseColor.r, pointLight->diffuseColor.g, pointLight->diffuseColor.b);
	glUniform1f(lightDiffuseIntensityUniformLocation, pointLight->diffuseIntensity);
	glUniform1f(lightSpecularIntensityUniformLocation, pointLight->specularIntensity);
	glUniform1f(lightSpecularPowerUniformLocation, pointLight->specularPower);
	//glUniform3f(lightAttenuationUniformLocation, pointLight->attenuation.x, pointLight->attenuation.y, pointLight->attenuation.z);
}

void PointLightingShader::setEyePositionUniform(const glm::vec3 &eyePosition)
{
	glUniform3f(eyePositionUniformLocation, eyePosition.x, eyePosition.y, eyePosition.z);
}

void PointLightingShader::setShadowBiasUniform(float shadowBias)
{
	glUniform1f(shadowBiasUniformLocation, shadowBias);
}