#include <gtc\type_ptr.hpp>

#include "UIDrawShader.hpp"
#include "..\Util\Error.hpp"

const std::string UIDrawShader::WORLD_MATRIX_UNIFORM_NAME = "worldMatrix";
const std::string UIDrawShader::UV_SCALE_UNIFORM_NAME = "uvScale";
const std::string UIDrawShader::COLOR_UNIFORM_NAME = "color";
const std::string UIDrawShader::COLOR_OVERRIDE_UNIFORM_NAME = "colorOverride";
const std::string UIDrawShader::MRT_RGB_OVERRIDE_UNIFORM_NAME = "mrtRGBReinterpretOverride";
const std::string UIDrawShader::MRT_A_OVERRIDE_UNIFORM_NAME = "mrtAReinterpretOverride";
const std::string UIDrawShader::MRT_SCALE_UNIFORM_NAME = "mrtScale";
const std::string UIDrawShader::DIFFUSE_MAP_UNIFORM_NAME = "diffuseMap";

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

	if (!getUniformLocation(COLOR_OVERRIDE_UNIFORM_NAME, colorOverrideUniformLocation))
		return false;

	if (!getUniformLocation(MRT_RGB_OVERRIDE_UNIFORM_NAME, mrtRGBOverrideUniformLocation))
		return false;

	if (!getUniformLocation(MRT_A_OVERRIDE_UNIFORM_NAME, mrtAOverrideUniformLocation))
		return false;

	if (!getUniformLocation(MRT_SCALE_UNIFORM_NAME, mrtScaleUniformLocation))
		return false;

	GLint diffuseMapUniformLocation;

	if (!getUniformLocation(DIFFUSE_MAP_UNIFORM_NAME, diffuseMapUniformLocation))
		return false;

	glUniform1i(diffuseMapUniformLocation, 0);

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

void UIDrawShader::setColorOverrideUniform(bool colorOverride)
{
	glUniform1i(colorOverrideUniformLocation, colorOverride);
}

void UIDrawShader::setMRTRGBOverrideUniform(bool mrtRGBOverride)
{
	glUniform1i(mrtRGBOverrideUniformLocation, mrtRGBOverride);
}

void UIDrawShader::setMRTAOverrideUniform(bool mrtAOverride)
{
	glUniform1i(mrtAOverrideUniformLocation, mrtAOverride);
}

void UIDrawShader::setMRTScaleUniform(float mrtScale)
{
	glUniform1f(mrtScaleUniformLocation, mrtScale);
}