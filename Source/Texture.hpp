#pragma once

#include <SDL_opengl.h>
#include <string>

class Texture
{
private:
	GLuint handle;
	std::string filename;
	unsigned int refCount;

public:
	~Texture();

	bool operator==(const Texture &other);

	bool load(const std::string &filename);
	void bind(GLenum textureUnit);

	unsigned int getRefCount();
	void setRefCount(unsigned int refCount);

	std::string getFilename();
};