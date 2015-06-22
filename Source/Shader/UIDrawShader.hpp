#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class UIDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string UV_SCALE_UNIFORM_NAME;
	static const std::string COLOR_UNIFORM_NAME;
	static const std::string COLOR_OVERRIDE_UNIFORM_NAME;
	static const std::string MRT_RGB_OVERRIDE_UNIFORM_NAME;
	static const std::string MRT_A_OVERRIDE_UNIFORM_NAME;
	static const std::string MRT_SCALE_UNIFORM_NAME;
	static const std::string DIFFUSE_MAP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation;
	GLint uvScaleUniformLocation;
	GLint colorUniformLocation;
	GLint colorOverrideUniformLocation;
	GLint mrtRGBOverrideUniformLocation;
	GLint mrtAOverrideUniformLocation;
	GLint mrtScaleUniformLocation;
	GLint diffuseMapUniformLocation;

public:
	bool create();

	void setWorldMatrixUniform(const glm::mat4 &worldMatrix);
	void setUVScaleUniform(const glm::vec2 &uvScale);
	void setColorUniform(const glm::vec4 &color);
	void setColorOverrideUniform(bool colorOverride);
	void setMRTRGBOverrideUniform(bool mrtRGBOverride);
	void setMRTAOverrideUniform(bool mrtAOverride);
	void setMRTScaleUniform(float mrtScale);
};