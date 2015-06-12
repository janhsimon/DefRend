#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"
#include "..\Light\PointLight.hpp"

class PointLightingShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	//static const std::string INV_WORLD_MATRIX_UNIFORM_NAME;
	//static const std::string CAMERA_FAR_CLIP_UNIFORM_NAME;
	static const std::string SCREEN_SIZE_UNIFORM_NAME;
	static const std::string LIGHT_POSITION_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_POWER_UNIFORM_NAME;
	//static const std::string LIGHT_ATTENUATION_UNIFORM_NAME;
	static const std::string EYE_POSITION_UNIFORM_NAME;
	static const std::string SHADOW_BIAS_UNIFORM_NAME;
	static const std::string GBUFFER_MAPS_UNIFORM_NAMES[2];
	static const std::string SHADOW_MAP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;
	GLint invWorldMatrixUniformLocation;
	GLint cameraFarClipUniformLocation;
	GLint screenSizeUniformLocation;
	GLint lightPositionUniformLocation;
	GLint lightDiffuseColorUniformLocation, lightDiffuseIntensityUniformLocation;
	GLint lightSpecularIntensityUniformLocation, lightSpecularPowerUniformLocation;
	//GLint lightAttenuationUniformLocation;
	GLint eyePositionUniformLocation;
	GLint shadowBiasUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	//void setCameraFarClipUniform(float cameraFarClip);
	void setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight);
	void setLightParameters(const PointLight *pointLight);
	void setEyePositionUniform(const glm::vec3 &eyePosition);
	void setShadowBiasUniform(float shadowBias);
};