#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"
#include "..\Light\DirectionalLight.hpp"
#include "..\Light\PointLight.hpp"
#include "..\Light\SpotLight.hpp"

class ForwardShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string LIGHTS_PASSED_UNIFORM_NAME;
	static const std::string EYE_POSITION_UNIFORM_NAME;
	static const std::string TEXTURE_MAP_UNIFORM_NAMES[4];

	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;
	GLint lightsPassedUniformLocation;
	GLint eyePositionUniformLocation;
	GLint lightTypeUniformLocation[33];
	GLint lightPositionUniformLocation[33];
	GLint lightDirectionUniformLocation[33];
	GLint lightDiffuseColorUniformLocation[33];
	GLint lightDiffuseIntensityUniformLocation[33];
	GLint lightSpecularIntensityUniformLocation[33];
	GLint lightSpecularPowerUniformLocation[33];
	GLint lightShadowBiasUniformLocation[33];
	GLint lightCutoffCosineUniformLocation[33];

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setLightsPassedUniform(int lightsPassed);
	void setEyePositionUniform(const glm::vec3 &eyePosition);
	void setLightParameterUniforms(int index, const DirectionalLight &pointLight);
};