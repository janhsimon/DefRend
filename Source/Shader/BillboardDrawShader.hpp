#include <glm.hpp>

#include "ShaderProgram.hpp"

class BillboardDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string DIFFUSE_MAP_UNIFORM_NAME;
	static const std::string TINT_COLOR_UNIFORM_NAME;

	GLint worldViewProjectionMatrixUniformLocation;
	GLint diffuseMapUniformLocation;
	GLint tintColorUniformLocation;

public:
	bool create();

	void setWorldViewProjectionMatrixUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setTintColorUniform(const glm::vec3 &tintColor);
};