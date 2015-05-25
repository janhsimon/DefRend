#include <glm.hpp>

#include "ShaderProgram.hpp"
#include "..\Light\SpotLight.hpp"

class SpotLightingShader : public ShaderProgram
{
private:
	static const std::string WORLD_VIEW_PROJECTION_MATRIX_UNIFORM_NAME;
	static const std::string SCREEN_SIZE_UNIFORM_NAME;
	static const std::string LIGHT_POSITION_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_COLOR_UNIFORM_NAME;
	static const std::string LIGHT_DIFFUSE_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_INTENSITY_UNIFORM_NAME;
	static const std::string LIGHT_SPECULAR_POWER_UNIFORM_NAME;
	static const std::string LIGHT_DIRECTION_UNIFORM_NAME;
	static const std::string LIGHT_CUTOFF_COSINE_UNIFORM_NAME;
	static const std::string LIGHT_ATTENUATION_UNIFORM_NAME;
	static const std::string EYE_POSITION_UNIFORM_NAME;
	static const std::string GBUFFER_MAPS_UNIFORM_NAMES[4];

	GLint worldViewProjectionMatrixUniformLocation;
	GLint screenSizeUniformLocation;
	GLint lightPositionUniformLocation;
	GLint lightDiffuseColorUniformLocation, lightDiffuseIntensityUniformLocation;
	GLint lightSpecularIntensityUniformLocation, lightSpecularPowerUniformLocation;
	GLint lightDirectionUniformLocation;
	GLint lightCutoffCosineUniformLocation;
	GLint lightAttenuationUniformLocation;
	GLint eyePositionUniformLocation;

public:
	bool create();

	void setWorldViewProjectionUniforms(const glm::mat4 &worldMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
	void setScreenSizeUniform(unsigned int screenWidth, unsigned int screenHeight);
	void setLightParameters(const SpotLight *spotLight);
	void setEyePositionUniform(const glm::vec3 &eyePosition);
};