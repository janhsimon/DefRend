#include <gtc\type_ptr.hpp>

#include "DirectionalLightingShader.hpp"
#include "..\Util\Error.hpp"

const std::string DirectionalLightingShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string DirectionalLightingShader::LIGHT_DIRECTION_UNIFORM_NAME = "lightDirection";
const std::string DirectionalLightingShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "lightDiffuseColor";
const std::string DirectionalLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string DirectionalLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string DirectionalLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[4] = { "gbuffer_worldPosition", "gbuffer_diffuse", "gbuffer_specular", "gbuffer_normal" };

bool DirectionalLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\DirectionalLighting.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(program);

	if (!getUniformLocation(WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME, worldViewProjectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIRECTION_UNIFORM_NAME, lightDirectionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_COLOR_UNIFORM_NAME, lightDiffuseColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME, lightDiffuseIntensityUniformLocation))
		return false;

	if (!getUniformLocation(SCREEN_SIZE_UNIFORM_NAME, screenSizeUniformLocation))
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
		Error::report("Error", "Failed to load directional lighting shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void DirectionalLightingShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldViewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr((projectionMatrix * viewMatrix) * worldMatrix));
}

void DirectionalLightingShader::setScreenSizeUniforms(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void DirectionalLightingShader::setLightParameters(const DirectionalLight *directionalLight)
{
	glUniform3f(lightDirectionUniformLocation, directionalLight->getForward().x, directionalLight->getForward().y, directionalLight->getForward().z);
	glUniform3f(lightDiffuseColorUniformLocation, directionalLight->diffuseColor.r, directionalLight->diffuseColor.g, directionalLight->diffuseColor.b);
	glUniform1f(lightDiffuseIntensityUniformLocation, directionalLight->diffuseIntensity);
}