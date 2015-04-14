#pragma once

#include <glew.h>
#include <string>
#include <SDL_opengl.h>

class Error
{
public:
	static void report(const std::string &title, const std::string &description);
	static const std::string getOpenGLErrorString(GLenum errorFlag);
};