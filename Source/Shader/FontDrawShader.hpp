#include <glm.hpp>

#include "ShaderProgram.hpp"

class FontDrawShader : public ShaderProgram
{
private:
	static const std::string WORLD_MATRIX_UNIFORM_NAME;
	static const std::string DIFFUSE_MAP_UNIFORM_NAME;

	GLint worldMatrixUniformLocation;
	GLint diffuseMapUniformLocation;

public:
	bool create();

	void setWorldMatrixUniform(const glm::mat4 &worldMatrix);
};