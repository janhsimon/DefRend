#include "Error.hpp"
#include "Texture.hpp"

#include <SDL_image.h>

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandle);
}

bool Texture::load(const std::string &filename)
{
	SDL_Surface *surface = IMG_Load(filename.c_str());

	if (!surface)
	{
		//Error::report("Error", "IMG_Load Error: " + std::string(SDL_GetError()));
		return false;
	}

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	
	SDL_FreeSurface(surface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}