#include <gtc\type_ptr.hpp>

#include "..\Util\Error.hpp"
#include "SpotLightingShader.hpp"

const std::string SpotLightingShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string SpotLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string SpotLightingShader::LIGHT_POSITION_UNIFORM_NAME = "lightPosition";
const std::string SpotLightingShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "lightDiffuseColor";
const std::string SpotLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string SpotLightingShader::LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME = "lightSpecularIntensity";
const std::string SpotLightingShader::LIGHT_SPECULAR_POWER_UNIFORM_NAME = "lightSpecularPower";
const std::string SpotLightingShader::LIGHT_DIRECTION_UNIFORM_NAME = "lightDirection";
const std::string SpotLightingShader::LIGHT_CUTOFF_COSINE_UNIFORM_NAME = "lightCutoffCosine";
const std::string SpotLightingShader::LIGHT_ATTENUATION_UNIFORM_NAME = "lightAttenuation";
const std::string SpotLightingShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";
const std::string SpotLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[4] = { "gbuffer_worldPosition", "gbuffer_diffuse", "gbuffer_specular", "gbuffer_normal" };

bool SpotLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\SpotLighting.fs.glsl"))
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

	if (!getUniformLocation(LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME, lightSpecularIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_POWER_UNIFORM_NAME, lightSpecularPowerUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIRECTION_UNIFORM_NAME, lightDirectionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_CUTOFF_COSINE_UNIFORM_NAME, lightCutoffCosineUniformLocation))
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
		Error::report("Error", "Failed to load spot lighting shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void SpotLightingShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldViewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr((projectionMatrix * viewMatrix) * worldMatrix));
}

void SpotLightingShader::setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void SpotLightingShader::setLightPositionUniform(float x, float y, float z)
{
	glUniform3f(lightPositionUniformLocation, x, y, z);
}

void SpotLightingShader::setLightDiffuseUniforms(float r, float g, float b, float intensity)
{
	glUniform3f(lightDiffuseColorUniformLocation, r, g, b);
	glUniform1f(lightDiffuseIntensityUniformLocation, intensity);
}

void SpotLightingShader::setLightSpecularUniforms(float intensity, float power)
{
	glUniform1f(lightSpecularIntensityUniformLocation, intensity);
	glUniform1f(lightSpecularPowerUniformLocation, power);
}

void SpotLightingShader::setLightDirectionUniform(float x, float y, float z)
{
	glUniform3f(lightDirectionUniformLocation, x, y, z);
}

void SpotLightingShader::setLightCutoffAngleUniform(float cutoffCosine)
{
	glUniform1f(lightCutoffCosineUniformLocation, glm::cos(glm::radians(cutoffCosine)));
}

void SpotLightingShader::setLightAttenuationUniform(float constant, float linear, float exponent)
{
	glUniform3f(lightAttenuationUniformLocation, constant, linear, exponent);
}

void SpotLightingShader::setEyePositionUniform(float x, float y, float z)
{
	glUniform3f(eyePositionUniformLocation, x, y, z);
}