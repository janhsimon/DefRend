#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"
#include "..\Light\DirectionalLight.hpp"

class DeferredSuperSlimShader : public ShaderProgram
{
private:
	// vertex shader uniform names
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string EYE_POSITION_UNIFORM_NAME;

	// vertex shader uniform locations
	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;
	GLint eyePositionUniformLocation;

	// fragment shader uniform names
	static const std::string GBUFFER_MAPS_UNIFORM_NAMES[2];
	static const std::string SHADOW_MAP_UNIFORM_NAME;
	static const std::string CAMERA_FAR_CLIP_UNIFORM_NAME;
	static const std::string SCREEN_SIZE_UNIFORM_NAME;
	static const std::string LIGHT_TYPE_UNIFORM_NAME;
	static const std::string LIGHT_POSITION_UNIFORM_NAME;
	static const std::string LIGHT_DIRECTION_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_POWER_UNIFORM_NAME;
	static const std::string LIGHT_CAST_SHADOWS_UNIFORM_NAME;
	static const std::string LIGHT_SHADOW_BIAS_UNIFORM_NAME;
	static const std::string LIGHT_CUTOFF_COSINE_UNIFORM_NAME;

	// fragment shader uniform locations
	GLint cameraFarClipUniformLocation;
	GLint screenSizeUniformLocation;
	GLint lightTypeUniformLocation;
	GLint lightPositionUniformLocation, lightDirectionUniformLocation;
	GLint lightDiffuseColorUniformLocation, lightDiffuseIntensityUniformLocation;
	GLint lightSpecularIntensityUniformLocation, lightSpecularPowerUniformLocation;
	GLint lightCastShadowsUniformLocation, lightShadowBiasUniformLocation;
	GLint lightCutoffCosineUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setEyePositionUniform(const glm::vec3 &eyePosition);
	void setCameraFarClip(float cameraFarClip);
	void setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight);
	void setLightParameterUniforms(const DirectionalLight &light);
};