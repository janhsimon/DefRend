#include <assert.h>

#include "GBufferFat.hpp"
#include "..\Util\Error.hpp"

GBufferFat::~GBufferFat()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(4, textures);
	glDeleteTextures(1, &depthTexture);
}

bool GBufferFat::load(unsigned int screenWidth, unsigned int screenHeight)
{
	// store type
	type = GBufferType::FAT;

	// create the FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create the gbuffer textures
	glGenTextures(4, textures);
	glGenTextures(1, &depthTexture);

	for (unsigned int i = 0; i < 4; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// set up our multiple rendertargets
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		Error::report("Error", "Failed to initialize fat G-Buffer FBO: " + Error::getOpenGLFrameBufferErrorString(error));
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

GLuint GBufferFat::getFBO()
{
	return FBO;
}

GLuint GBufferFat::getTexture(unsigned int index)
{
	assert(index >= 0 && index < 4);
	return textures[index];
}

GLuint GBufferFat::getDepthTexture()
{
	return depthTexture;
}