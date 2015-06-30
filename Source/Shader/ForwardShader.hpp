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
	GLint lightTypeUniformLocation[8];
	GLint lightPositionUniformLocation[8];
	GLint lightDirectionUniformLocation[8];
	GLint lightDiffuseColorUniformLocation[8];
	GLint lightDiffuseIntensityUniformLocation[8];
	GLint lightSpecularIntensityUniformLocation[8];
	GLint lightSpecularPowerUniformLocation[8];
	GLint lightShadowBiasUniformLocation[8];
	GLint lightCutoffCosineUniformLocation[8];

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setLightsPassedUniform(int lightsPassed);
	void setEyePositionUniform(const glm::vec3 &eyePosition);
	void setLightParameterUniforms(int index, const DirectionalLight &pointLight);
};