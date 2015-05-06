#pragma once

#include <glew.h>
#include <SDL_opengl.h>
#include <string>

class ShaderProgram
{
private:
	std::string filename;
	GLuint program;
	GLuint vertexShader, geometryShader, fragmentShader;

	bool loadShader(GLuint shader, const std::string &filename);
	bool compileShader(GLuint shader, const std::string &filename);

public:
	~ShaderProgram();

	bool load(const std::string &vertexShaderFilename, const std::string &geometryShaderFilename, const std::string &fragmentShaderFilename);
	bool link();
	bool getUniformLocation(const std::string &uniformName, GLint &locationOut);
	GLuint getProgram();
	GLuint getVertexShader();
	GLuint getGeometryShader();
	GLuint getFragmentShader();
};