#include "ShaderProgram.hpp"

#include <glm.hpp>

class PointLightingShader : public ShaderProgram
{
private:
	static const std::string WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string SCREEN_SIZE_UNIFORM_NAME;
	static const std::string LIGHT_POSITION_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_POWER_UNIFORM_NAME;
	static const std::string LIGHT_ATTENUATION_UNIFORM_NAME;
	static const std::string EYE_POSITION_UNIFORM_NAME;
	static const std::string GBUFFER_MAPS_UNIFORM_NAMES[4];

	GLint worldViewProjectionMatrixUniformLocation;
	GLint screenSizeUniformLocation;
	GLint lightPositionUniformLocation;
	GLint lightDiffuseColorUniformLocation, lightDiffuseIntensityUniformLocation;
	GLint lightSpecularColorUniformLocation, lightSpecularIntensityUniformLocation, lightSpecularPowerUniformLocation;
	GLint lightAttenuationUniformLocation;
	GLint eyePositionUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setScreenSizeUniforms(unsigned int screenWidth, unsigned int screenHeight);
	void setLightPositionUniforms(float x, float y, float z);
	void setLightDiffuseUniforms(float r, float g, float b, float intensity);
	void setLightSpecularUniforms(float r, float g, float b, float intensity, float power);
	void setLightAttenuationUniforms(float exponential, float linear, float constant);
	void setEyePositionUniforms(float x, float y, float z);
};