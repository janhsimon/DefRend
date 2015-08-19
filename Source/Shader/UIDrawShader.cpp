#include <gtc\type_ptr.hpp>

#include "UIDrawShader.hpp"
#include "..\Util\Error.hpp"

const std::string UIDrawShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string UIDrawShader::UV_SCALE_UNIFORM_NAME = "uvScale";
const std::string UIDrawShader::COLOR_UNIFORM_NAME = "color";
const std::string UIDrawShader::MODE_UNIFORM_NAME = "mode";
const std::string UIDrawShader::GBUFFER_LAYOUT_UNIFORM_NAME = "gBufferLayout";
const std::string UIDrawShader::TEXTURE_MAP_UNIFORM_NAME = "textureMap";
const std::string UIDrawShader::UINT_TEXTURE_MAP_UNIFORM_NAME = "uintTextureMap";
const std::string UIDrawShader::CAMERA_FAR_CLIP_UNIFORM_NAME = "farClip";

bool UIDrawShader::create()
{
	if (!load("Shaders\\UIDraw.vs.glsl", "", "Shaders\\UIDraw.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(program);

	if (!getUniformLocation(WORLD_MATRIX_UNIFORM_NAME, worldMatrixUniformLocation))
		return false;

	if (!getUniformLocation(UV_SCALE_UNIFORM_NAME, uvScaleUniformLocation))
		return false;

	if (!getUniformLocation(COLOR_UNIFORM_NAME, colorUniformLocation))
		return false;

	if (!getUniformLocation(MODE_UNIFORM_NAME, modeUniformLocation))
		return false;

	if (!getUniformLocation(GBUFFER_LAYOUT_UNIFORM_NAME, gBufferLayoutUniformName))
		return false;

	if (!getUniformLocation(CAMERA_FAR_CLIP_UNIFORM_NAME, cameraFarClipUniformLocation))
		return false;

	GLint uintTextureMapUniformLocation, textureMapUniformLocation;

	if (!getUniformLocation(TEXTURE_MAP_UNIFORM_NAME, textureMapUniformLocation))
		return false;

	glUniform1i(textureMapUniformLocation, 0);

	if (!getUniformLocation(UINT_TEXTURE_MAP_UNIFORM_NAME, uintTextureMapUniformLocation))
		return false;

	glUniform1i(uintTextureMapUniformLocation, 1);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load UI draw shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void UIDrawShader::setWorldMatrixUniform(const glm::mat4 &worldMatrix)
{
	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));
}

void UIDrawShader::setUVScaleUniform(const glm::vec2 &uv)
{
	glUniform2f(uvScaleUniformLocation, uv.x, uv.y);
}

void UIDrawShader::setColorUniform(const glm::vec4 &color)
{
	glUniform4f(colorUniformLocation, color.r, color.g, color.b, color.a);
}

void UIDrawShader::setModeUniform(int mode)
{
	glUniform1i(modeUniformLocation, mode);
}

void UIDrawShader::setGBufferLayoutUniform(int layout)
{
	glUniform1i(gBufferLayoutUniformName, layout);
}

void UIDrawShader::setCameraFarClip(float farClip)
{
	glUniform1f(cameraFarClipUniformLocation, farClip);
}