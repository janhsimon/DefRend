#pragma once

#include <glew.h>
#include <SDL_opengl.h>
#include <string>

class Texture
{
private:
	GLuint handle;

public:
	std::string filename;
	unsigned int refCount;

	~Texture();

	bool operator==(const Texture &other);

	bool load(const std::string &filename);
	void bind(GLenum textureUnit);
	void unbind(GLenum textureUnit);
};