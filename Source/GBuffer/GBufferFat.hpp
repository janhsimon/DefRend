#pragma once

#include <glew.h>

#include "IGBuffer.hpp"

class GBufferFat : public IGBuffer
{
private:
	GLuint FBO;
	GLuint textures[4];
	GLuint depthTexture;

public:
	~GBufferFat();

	bool load(unsigned int screenWidth, unsigned int screenHeight);

	GLuint getFBO();
	GLuint getTexture(unsigned int index);
	GLuint getDepthTexture();
};