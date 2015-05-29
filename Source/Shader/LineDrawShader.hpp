#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class LineDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string COLOR_UNIFORM_NAME;

	GLint worldViewProjectionMatrixUniformLocation;
	GLint colorUniformLocation;

public:
	bool create();

	void setWorldViewProjectionMatrixUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setColorUniform(const glm::vec3 &color);
};