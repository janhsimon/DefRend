#pragma once

#include <glm.hpp>

#include "ShaderProgram.hpp"

class UIDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string UV_SCALE_UNIFORM_NAME;
	static const std::string COLOR_UNIFORM_NAME;
	static const std::string MODE_UNIFORM_NAME;
	static const std::string GBUFFER_LAYOUT_UNIFORM_NAME;
	static const std::string CAMERA_FAR_CLIP_UNIFORM_NAME;
	static const std::string TEXTURE_MAP_UNIFORM_NAME;
	static const std::string UINT_TEXTURE_MAP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation;
	GLint uvScaleUniformLocation;
	GLint colorUniformLocation;
	GLint modeUniformLocation;
	GLint gBufferLayoutUniformName;
	GLint cameraFarClipUniformLocation;

public:
	bool create();

	void setWorldMatrixUniform(const glm::mat4 &worldMatrix);
	void setUVScaleUniform(const glm::vec2 &uvScale);
	void setColorUniform(const glm::vec4 &color);
	void setModeUniform(int mode);
	void setGBufferLayoutUniform(int layout);
	void setCameraFarClip(float farClip);
};