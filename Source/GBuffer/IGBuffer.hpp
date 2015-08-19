#pragma once

#include <glew.h>

enum GBufferType
{
	SUPER_SLIM,
	SLIM,
	FAT
};

class IGBuffer
{
public:
	GBufferType type;

	virtual ~IGBuffer() {};

	virtual bool load(unsigned int screenWidth, unsigned int screenHeight) = 0;

	virtual GLuint getFBO() = 0;
	virtual GLuint getTexture(unsigned int index) = 0;
	virtual GLuint getDepthTexture() = 0;
};