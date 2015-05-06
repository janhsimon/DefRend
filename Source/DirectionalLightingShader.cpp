#include "Error.hpp"
#include "DirectionalLightingShader.hpp"

#include <gtc/type_ptr.hpp>

const std::string DirectionalLightingShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string DirectionalLightingShader::LIGHT_DIRECTION_UNIFORM_NAME = "lightDirection";
const std::string DirectionalLightingShader::LIGHT_AMBIENT_COLOR_UNIFORM_NAME = "lightAmbientColor";
const std::string DirectionalLightingShader::LIGHT_AMBIENT_INTENSITY_UNIFORM_NAME = "lightAmbientIntensity";
const std::string DirectionalLightingShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "lightDiffuseIntensity";
const std::string DirectionalLightingShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string DirectionalLightingShader::GBUFFER_MAPS_UNIFORM_NAMES[4] = { "gbuffer_worldPosition", "gbuffer_diffuse", "gbuffer_specular", "gbuffer_normal" };

bool DirectionalLightingShader::create()
{
	if (!load("Shaders\\Lighting.vs.glsl", "", "Shaders\\DirectionalLighting.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(getProgram());

	if (!getUniformLocation(WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME, worldViewProjectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIRECTION_UNIFORM_NAME, lightDirectionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_AMBIENT_COLOR_UNIFORM_NAME, lightAmbientColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_AMBIENT_INTENSITY_UNIFORM_NAME, lightAmbientIntensityUniformLocation))
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

void DirectionalLightingShader::setLightDirectionUniform(float x, float y, float z)
{
	glUniform3f(lightDirectionUniformLocation, x, y, z);
}

void DirectionalLightingShader::setLightAmbientUniforms(float r, float g, float b, float intensity)
{
	glUniform3f(lightAmbientColorUniformLocation, r, g, b);
	glUniform1f(lightAmbientIntensityUniformLocation, intensity);
}

void DirectionalLightingShader::setLightDiffuseUniform(float intensity)
{
	glUniform1f(lightDiffuseIntensityUniformLocation, intensity);
}

void DirectionalLightingShader::setScreenSizeUniforms(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}