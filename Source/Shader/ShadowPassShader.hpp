#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class ShadowPassShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string OPACITY_MAP_UNIFORM_NAME;
	static const std::string POINT_LIGHT_POSITION_UNIFORM_NAME;
	static const std::string CAMERA_FAR_CLIP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;
	GLint pointLightPositionUniformLocation;
	GLint cameraFarClipUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setPointLightPositionUniform(const glm::vec3 &pointLightPosition);
	void setCameraFarClipUniform(float cameraFarClip);
};