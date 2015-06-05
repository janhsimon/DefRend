#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class GeometryShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string CAMERA_FAR_CLIP_UNIFORM_NAME;
	static const std::string TEXTURE_MAP_UNIFORM_NAMES[4];

	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;
	GLint cameraFarClipUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setCameraFarClipUniform(float cameraFarClip);
};