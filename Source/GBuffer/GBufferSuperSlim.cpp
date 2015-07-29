#include <assert.h>

#include "GBufferSuperSlim.hpp"
#include "..\Util\Error.hpp"

GBufferSuperSlim::~GBufferSuperSlim()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(2, textures);
	glDeleteTextures(1, &depthTexture);
}

bool GBufferSuperSlim::load(unsigned int screenWidth, unsigned int screenHeight)
{
	// store type
	type = GBufferType::SUPER_SLIM;

	// create the FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create the gbuffer textures
	glGenTextures(2, textures);
	glGenTextures(1, &depthTexture);

	for (unsigned int i = 0; i < 2; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, screenWidth, screenHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, NULL);

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
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		Error::report("Error", "Failed to initialize super slim G-Buffer FBO: " + Error::getOpenGLFrameBufferErrorString(error));
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

GLuint GBufferSuperSlim::getFBO()
{
	return FBO;
}

GLuint GBufferSuperSlim::getTexture(unsigned int index)
{
	assert(index >= 0 && index < 2);
	return textures[index];
}

GLuint GBufferSuperSlim::getDepthTexture()
{
	return depthTexture;
}