#include "Error.hpp"
#include "PointLightingShader.hpp"

#include <gtc/type_ptr.hpp>

const std::string PointLightingShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string PointLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string PointLightingShader::LIGHT_POSITION_UNIFORM_NAME = "lightPosition";
const std::string PointLightingShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "lightDiffuseColor";
const std::string PointLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string PointLightingShader::LIGHT_SPECULAR_COLOR_UNIFORM_NAME = "lightSpecularColor";
const std::string PointLightingShader::LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME = "lightSpecularIntensity";
const std::string PointLightingShader::LIGHT_SPECULAR_POWER_UNIFORM_NAME = "lightSpecularPower";
const std::string PointLightingShader::LIGHT_ATTENUATION_UNIFORM_NAME = "lightAttenuation";
const std::string PointLightingShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";
const std::string PointLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[4] = { "gbuffer_worldPosition", "gbuffer_diffuse", "gbuffer_specular", "gbuffer_normal" };

bool PointLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\PointLighting.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(getProgram());

	if (!getUniformLocation(WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME, worldViewProjectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(SCREEN_SIZE_UNIFORM_NAME, screenSizeUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_POSITION_UNIFORM_NAME, lightPositionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_COLOR_UNIFORM_NAME, lightDiffuseColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME, lightDiffuseIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_COLOR_UNIFORM_NAME, lightSpecularColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME, lightSpecularIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_POWER_UNIFORM_NAME, lightSpecularPowerUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_ATTENUATION_UNIFORM_NAME, lightAttenuationUniformLocation))
		return false;

	if (!getUniformLocation(EYE_POSITION_UNIFORM_NAME, eyePositionUniformLocation))
		return false;

	GLint gBufferMapsUniformLocation;

	for (int i = 0; i < 4; ++i)
	{
		if (!getUniformLocation(GBUFFER_MAPS_UNIFORM_NAMES[i], gBufferMapsUniformLocation))
			return false;

		glUniform1i(gBufferMapsUniformLocation, i);
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load point lighting shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void PointLightingShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldViewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr((projectionMatrix * viewMatrix) * worldMatrix));
}

void PointLightingShader::setScreenSizeUniforms(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void PointLightingShader::setLightPositionUniforms(float x, float y, float z)
{
	glUniform3f(lightPositionUniformLocation, x, y, z);
}

void PointLightingShader::setLightDiffuseUniforms(float r, float g, float b, float intensity)
{
	glUniform3f(lightDiffuseColorUniformLocation, r, g, b);
	glUniform1f(lightDiffuseIntensityUniformLocation, intensity);
}

void PointLightingShader::setLightSpecularUniforms(float r, float g, float b, float intensity, float power)
{
	glUniform3f(lightSpecularColorUniformLocation, r, g, b);
	glUniform1f(lightSpecularIntensityUniformLocation, intensity);
	glUniform1f(lightSpecularPowerUniformLocation, power);
}

void PointLightingShader::setLightAttenuationUniforms(float exponential, float linear, float constant)
{
	glUniform3f(lightAttenuationUniformLocation, exponential, linear, constant);
}

void PointLightingShader::setEyePositionUniforms(float x, float y, float z)
{
	glUniform3f(eyePositionUniformLocation, x, y, z);
}