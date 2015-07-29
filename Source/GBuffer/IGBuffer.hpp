#pragma once

#include <glew.h>

enum GBufferType
{
	FAT,
	SLIM,
	SUPER_SLIM
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