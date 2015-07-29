#include <gtc\type_ptr.hpp>

#include "DeferredSuperSlimShader.hpp"
#include "..\Light\PointLight.hpp"
#include "..\Light\SpotLight.hpp"
#include "..\Util\Error.hpp"

// vertex shader uniform names
const std::string DeferredSuperSlimShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string DeferredSuperSlimShader::VIEW_MATRIX_UNIFORM_NAME = "viewMatrix";
const std::string DeferredSuperSlimShader::PROJECTION_MATRIX_UNIFORM_NAME = "projectionMatrix";
const std::string DeferredSuperSlimShader::EYE_POSITION_UNIFORM_NAME = "eyePosition";

// fragment shader uniform names
const std::string DeferredSuperSlimShader::GBUFFER_MAPS_UNIFORM_NAMES[2] = { "inGBufferMRT0", "inGBufferMRT1" };
const std::string DeferredSuperSlimShader::SHADOW_MAP_UNIFORM_NAME = "inShadowMap";
const std::string DeferredSuperSlimShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "cameraFarClip";
const std::string DeferredSuperSlimShader::SCREEN_SIZE_UNIFORM_NAME = "screenSize";
const std::string DeferredSuperSlimShader::LIGHT_TYPE_UNIFORM_NAME = "light.type";
const std::string DeferredSuperSlimShader::LIGHT_POSITION_UNIFORM_NAME = "light.position";
const std::string DeferredSuperSlimShader::LIGHT_DIRECTION_UNIFORM_NAME = "light.direction";
const std::string DeferredSuperSlimShader::LIGHT_DIFFUSE_COLOR_UNIFORM_NAME = "light.diffuseColor";
const std::string DeferredSuperSlimShader::LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME = "light.diffuseIntensity";
const std::string DeferredSuperSlimShader::LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME = "light.specularIntensity";
const std::string DeferredSuperSlimShader::LIGHT_SPECULAR_POWER_UNIFORM_NAME = "light.specularPower";
const std::string DeferredSuperSlimShader::LIGHT_CAST_SHADOWS_UNIFORM_NAME = "light.castShadows";
const std::string DeferredSuperSlimShader::LIGHT_SHADOW_BIAS_UNIFORM_NAME = "light.shadowBias";
const std::string DeferredSuperSlimShader::LIGHT_CUTOFF_COSINE_UNIFORM_NAME = "light.cutoffCosine";

bool DeferredSuperSlimShader::create()
{
	if (!load("Shaders\\Deferred.vs.glsl", "", "Shaders\\DeferredSuperSlim.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(program);


	// vertex shader uniform names

	if (!getUniformLocation(WORLD_MATRIX_UNIFORM_NAME, worldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(VIEW_MATRIX_UNIFORM_NAME, viewMatrixUniformLocation))
		return false;

	if (!getUniformLocation(PROJECTION_MATRIX_UNIFORM_NAME, projectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(EYE_POSITION_UNIFORM_NAME, eyePositionUniformLocation))
		return false;


	// fragment shader uniform names

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

	if (!getUniformLocation(SCREEN_SIZE_UNIFORM_NAME, screenSizeUniformLocation))
		return false;

	if (!getUniformLocation(CAMERA_FAR_CLIP_UNIFORM_NAME, cameraFarClipUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_TYPE_UNIFORM_NAME, lightTypeUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_POSITION_UNIFORM_NAME, lightPositionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIRECTION_UNIFORM_NAME, lightDirectionUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_COLOR_UNIFORM_NAME, lightDiffuseColorUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME, lightDiffuseIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME, lightSpecularIntensityUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SPECULAR_POWER_UNIFORM_NAME, lightSpecularPowerUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_CAST_SHADOWS_UNIFORM_NAME, lightCastShadowsUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_SHADOW_BIAS_UNIFORM_NAME, lightShadowBiasUniformLocation))
		return false;

	if (!getUniformLocation(LIGHT_CUTOFF_COSINE_UNIFORM_NAME, lightCutoffCosineUniformLocation))
		return false;
	

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load slim deferred shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void DeferredSuperSlimShader::setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void DeferredSuperSlimShader::setEyePositionUniform(const glm::vec3 &eyePosition)
{
	glUniform3f(eyePositionUniformLocation, eyePosition.x, eyePosition.y, eyePosition.z);
}

void DeferredSuperSlimShader::setCameraFarClip(float cameraFarClip)
{
	glUniform1f(cameraFarClipUniformLocation, cameraFarClip);
}

void DeferredSuperSlimShader::setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight)
{
	glUniform2f(screenSizeUniformLocation, (float)screenWidth, (float)screenHeight);
}

void DeferredSuperSlimShader::setLightParameterUniforms(const DirectionalLight &light)
{
	glUniform1i(lightTypeUniformLocation, light.type);
	glUniform3f(lightPositionUniformLocation, light.position.x, light.position.y, light.position.z);
	glUniform3f(lightDirectionUniformLocation, light.getForward().x, light.getForward().y, light.getForward().z);
	glUniform3f(lightDiffuseColorUniformLocation, light.diffuseColor.r, light.diffuseColor.g, light.diffuseColor.b);
	glUniform1f(lightDiffuseIntensityUniformLocation, light.diffuseIntensity);

	if (light.type == LightType::POINT_LIGHT || light.type == LightType::SPOT_LIGHT)
	{
		PointLight *p = (PointLight*)&light;

		glUniform1f(lightSpecularIntensityUniformLocation, p->specularIntensity);
		glUniform1f(lightSpecularPowerUniformLocation, p->specularPower);
		glUniform1i(lightCastShadowsUniformLocation, p->getCastShadows());
		glUniform1f(lightShadowBiasUniformLocation, p->shadowBias);

		if (light.type == LightType::SPOT_LIGHT)
		{
			SpotLight *s = (SpotLight*)p;
			glUniform1f(lightCutoffCosineUniformLocation, glm::cos(glm::radians(s->cutoffAngle)));
		}
	}
}

