#include <gtc\type_ptr.hpp>

#include "BillboardDrawShader.hpp"
#include "..\Util\Error.hpp"

const std::string BillboardDrawShader::WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME = "worldViewProjectionMatrix";
const std::string BillboardDrawShader::DIFFUSE_MAP_UNIFORM_NAME = "diffuseMap";
const std::string BillboardDrawShader::TINT_COLOR_UNIFORM_NAME = "tintColor";

bool BillboardDrawShader::create()
{
	if (!load("Shaders\\BillboardDraw.vs.glsl", "", "Shaders\\BillboardDraw.fs.glsl"))
		return false;

	if (!link())
		return false;

	glUseProgram(getProgram());

	if (!getUniformLocation(WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME, worldViewProjectionMatrixUniformLocation))
		return false;

	if (!getUniformLocation(DIFFUSE_MAP_UNIFORM_NAME, diffuseMapUniformLocation))
		return false;

	if (!getUniformLocation(TINT_COLOR_UNIFORM_NAME, tintColorUniformLocation))
		return false;

	glUniform1i(diffuseMapUniformLocation, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load billboard draw shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void BillboardDrawShader::setWorldViewProjectionMatrixUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glUniformMatrix4fv(worldViewProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr((projectionMatrix * viewMatrix) * worldMatrix));
}

void BillboardDrawShader::setTintColorUniform(float r, float g, float b)
{
	glUniform3f(tintColorUniformLocation, r, g, b);
}