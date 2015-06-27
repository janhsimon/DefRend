#include <sstream>
#include <gtc\type_ptr.hpp>

#include "ForwardShader.hpp"
#include "..\Util\Error.hpp"

const std::string ForwardShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string ForwardShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string ForwardShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
const std::string ForwardShader::LIGHTS_PASSED_UNIFORM_NAME = "lightsPassed";
const std::string ForwardShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";
const std::string ForwardShader::TEXTURE_MAP_UNIFORM_NAMES[4] = { "diffuseMap", "specularMap", "normalMap", "opacityMap" };

bool ForwardShader::create()
{
	if (!load("Shaders\\Forward.vs.glsl", "", "Shaders\\Forward.fs.glsl"))
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

	if (!getUniformLocation(LIGHTS_PASSED_UNIFORM_NAME, lightsPassedUniformLocation))
		return false;

	if (!getUniformLocation(EYE_POSITION_UNIFORM_NAME, eyePositionUniformLocation))
		return false;

	for (int i = 0; i < 8; ++i)
	{
		std::stringstream uniformBaseName;
		uniformBaseName << "light[" << i << "]";

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".type").c_str(), lightTypeUniformLocation[i]))
			return false;

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".position").c_str(), lightPositionUniformLocation[i]))
			return false;

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".diffuseColor").c_str(), lightDiffuseColorUniformLocation[i]))
			return false;

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".diffuseIntensity").c_str(), lightDiffuseIntensityUniformLocation[i]))
			return false;

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".specularIntensity").c_str(), lightSpecularIntensityUniformLocation[i]))
			return false;

		if (!getUniformLocation(std::string(uniformBaseName.str() + ".specularPower").c_str(), lightSpecularPowerUniformLocation[i]))
			return false;

		//getUniformLocation(std::string(uniformBaseName.str() + ".shadowBias").c_str(), lightShadowBiasUniformLocation[i]);
	}

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
		Error::report("Error", "Failed to load forward shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void ForwardShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void ForwardShader::setLightsPassedUniform(int lightsPassed)
{
	assert(lightsPassed >= 0 || lightsPassed < 8);

	glUniform1i(lightsPassedUniformLocation, lightsPassed);
}

void ForwardShader::setEyePositionUniform(const glm::vec3 &eyePosition)
{
	glUniform3f(eyePositionUniformLocation, eyePosition.x, eyePosition.y, eyePosition.z);
}

void ForwardShader::setLightParameterUniforms(int index, const DirectionalLight &light)
{
	assert(index >= 0 || index < 8);

	glUniform1i(lightTypeUniformLocation[index], light.type);
	glUniform3f(lightPositionUniformLocation[index], light.position.x, light.position.y, light.position.z);
	glUniform3f(lightDiffuseColorUniformLocation[index], light.diffuseColor.r, light.diffuseColor.g, light.diffuseColor.b);
	glUniform1f(lightDiffuseIntensityUniformLocation[index], light.diffuseIntensity);

	if (light.type == LightType::POINT_LIGHT || light.type == LightType::SPOT_LIGHT)
	{
		PointLight *p = (PointLight*)&light;

		glUniform1f(lightSpecularIntensityUniformLocation[index], p->specularIntensity);
		glUniform1f(lightSpecularPowerUniformLocation[index], p->specularPower);

		//glUniform1f(lightShadowBiasUniformLocation[index], light.shadowBias);
	}
}