#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class UIDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string UV_SCALE_UNIFORM_NAME;
	static const std::string DIFFUSE_MAP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation;
	GLint uvScaleUniformLocation;
	GLint diffuseMapUniformLocation;

public:
	bool create();

	void setWorldMatrixUniform(const glm::mat4 &worldMatrix);
	void setUVScaleUniform(const glm::vec2 &uvScale);
};