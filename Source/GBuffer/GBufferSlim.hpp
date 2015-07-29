#pragma once

#include <glew.h>

#include "IGBuffer.hpp"

class GBufferSlim : public IGBuffer
{
private:
	GLuint FBO;
	GLuint textures[2];
	GLuint depthTexture;

public:
	~GBufferSlim();

	bool load(unsigned int screenWidth, unsigned int screenHeight);

	GLuint getFBO();
	GLuint getTexture(unsigned int index);
	GLuint getDepthTexture();
};