#include <glew.h>

#include "GBuffer.hpp"
#include "..\Util\Error.hpp"

GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(4, textures);
	glDeleteTextures(1, &depthTexture);
}

bool GBuffer::load(unsigned int screenWidth, unsigned int screenHeight)
{
	// create the FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

	// create the gbuffer textures
	glGenTextures(4, textures);
	glGenTextures(1, &depthTexture);

	for (unsigned int i = 0; i < 4; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		//if (i == 1)
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		//else
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// set up our multiple rendertargets
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		Error::report("Error", "Failed to initialize frame buffer object: " + Error::getOpenGLFrameBufferErrorString(error));
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

/*
void GBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void GBuffer::bindForReading(bool debug)
{
	if (debug)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	else
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		for (unsigned int i = 0; i < 4; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
	}
}

void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE textureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}
*/