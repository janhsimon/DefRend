#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"
#include "..\Light\DirectionalLight.hpp"

class DirectionalLightingShader : public ShaderProgram
{
private:
	static const std::string WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string LIGHT_DIRECTION_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME;
	static const std::string SCREEN_SIZE_UNIFORM_NAME;
	static const std::string GBUFFER_MAPS_UNIFORM_NAMES[4];

	GLint worldViewProjectionMatrixUniformLocation;
	GLint lightDirectionUniformLocation;
	GLint lightDiffuseColorUniformLocation;
	GLint lightDiffuseIntensityUniformLocation;
	GLint screenSizeUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setScreenSizeUniforms(unsigned int screenWidth, unsigned int screenHeight);
	void setLightParameters(const DirectionalLight *directionalLight);
};