#include <gtc\type_ptr.hpp>

#include "..\Util\Error.hpp"
#include "SpotLightingShader.hpp"

const std::string SpotLightingShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string SpotLightingShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string SpotLightingShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
//const std::string SpotLightingShader::INV_WORLD_MATRIX_UNIFORM_NAME = "invWorldMatrix";
//const std::string SpotLightingShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "cameraFarClip";
const std::string SpotLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string SpotLightingShader::LIGHT_POSITION_UNIFORM_NAME = "lightPosition";
const std::string SpotLightingShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "lightDiffuseColor";
const std::string SpotLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string SpotLightingShader::LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME = "lightSpecularIntensity";
const std::string SpotLightingShader::LIGHT_SPECULAR_POWER_UNIFORM_NAME = "lightSpecularPower";
const std::string SpotLightingShader::LIGHT_DIRECTION_UNIFORM_NAME = "lightDirection";
const std::string SpotLightingShader::LIGHT_CUTOFF_COSINE_UNIFORM_NAME = "lightCutoffCosine";
//const std::string SpotLightingShader::LIGHT_ATTENUATION_UNIFORM_NAME = "lightAttenuation";
const std::string SpotLightingShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";
const std::string SpotLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[2] = { "inGBufferMRT0", "inGBufferMRT1" };

bool SpotLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\SpotLighting.fs.glsl"))
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

	if (!getUniformLocation(LIGHT_DIRECTION_UNIFORM_NAME, lightDirectionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_CUTOFF_COSINE_UNIFORM_NAME, lightCutoffCosineUniformLocation))
		return false;

	//if (!getUniformLocation(LIGHT_ATTENUATION_UNIFORM_NAME, lightAttenuationUniformLocation))
		//return false;

	if (!getUniformLocation(EYE_POSITION_UNIFORM_NAME, eyePositionUniformLocation))
		return false;

	GLint gBufferMapsUniformLocation;

	for (int i = 0; i < 2; ++i)
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

/*
void SpotLightingShader::setCameraFarClipUniform(float cameraFarClip)
{
	glUniform1f(cameraFarClipUniformLocation, cameraFarClip);
}
*/

void SpotLightingShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//glUniformMatrix4fv(invWorldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::inverse(viewMatrix)));
}

void SpotLightingShader::setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void SpotLightingShader::setLightParameters(const SpotLight *spotLight)
{
	assert(spotLight);

	glUniform3f(lightPositionUniformLocation, spotLight->position.x, spotLight->position.y, spotLight->position.z);
	glUniform3f(lightDiffuseColorUniformLocation, spotLight->diffuseColor.r, spotLight->diffuseColor.g, spotLight->diffuseColor.b);
	glUniform1f(lightDiffuseIntensityUniformLocation, spotLight->diffuseIntensity);
	glUniform1f(lightSpecularIntensityUniformLocation, spotLight->specularIntensity);
	glUniform1f(lightSpecularPowerUniformLocation, spotLight->specularPower);
	glUniform3f(lightDirectionUniformLocation, spotLight->getForward().x, spotLight->getForward().y, spotLight->getForward().z);
	glUniform1f(lightCutoffCosineUniformLocation, glm::cos(glm::radians(spotLight->cutoffAngle)));
	//glUniform3f(lightAttenuationUniformLocation, spotLight->attenuation.x, spotLight->attenuation.y, spotLight->attenuation.z);
}

void SpotLightingShader::setEyePositionUniform(const glm::vec3 &eyePosition)
{
	glUniform3f(eyePositionUniformLocation, eyePosition.x, eyePosition.y, eyePosition.z);
}