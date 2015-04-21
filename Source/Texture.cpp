#include "Error.hpp"
#include "Texture.hpp"

#include <SDL_image.h>
#include <sstream>

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandle);
}

bool Texture::operator==(const Texture &other)
{
	if (filename.compare(other.filename) == 0)
		return true;
	
	return false;
}

bool Texture::load(const std::string &filename)
{
	SDL_Surface *surface = IMG_Load(filename.c_str());

	if (!surface)
	{
		Error::report("Error", "Failed to load texture \"" + filename + "\": " + std::string(SDL_GetError()));
		return false;
	}

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	int sourceFormat, destinationFormat;

	if (surface->format->BytesPerPixel == 1)
	{
		sourceFormat = GL_RED;
		destinationFormat = GL_R8;
	}
	else if (surface->format->BytesPerPixel == 3)
	{
		sourceFormat = GL_BGR;
		destinationFormat = GL_RGB8;
	}
	else if (surface->format->BytesPerPixel == 4)
	{
		sourceFormat = GL_BGRA;
		destinationFormat = GL_RGBA8;
	}
	else
	{
		std::stringstream s;
		s << "Invalid color depth (" << (int)surface->format->BytesPerPixel << " byte(s) per pixel) on texture \"" << filename + "\".";
		Error::report("Error", s.str());
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, destinationFormat, surface->w, surface->h, 0, sourceFormat, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to upload data to GPU for texture \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->filename = filename;

	refCount = 1;

	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}

unsigned int Texture::getRefCount()
{
	return refCount;
}

void Texture::setRefCount(unsigned int refCount)
{
	this->refCount = refCount;
}

std::string Texture::getFilename()
{
	return filename;
}