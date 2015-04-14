#pragma once

#include <string>

class Texture
{
private:
	GLuint textureHandle;

public:
	~Texture();

	bool load(const std::string &filename);
	void bind(GLenum textureUnit);
};