#pragma once

#include <glew.h>

class GBuffer
{
public:
	GLuint FBO;
	GLuint textures[2];
	GLuint depthTexture;

	~GBuffer();

	bool load(unsigned int screenWidth, unsigned int screenHeight);
};