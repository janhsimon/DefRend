#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class GeometrySlimShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string VIEW_MATRIX_UNIFORM_NAME;
	static const std::string PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string TEXTURE_MAP_UNIFORM_NAMES[4];

	GLint worldMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
};