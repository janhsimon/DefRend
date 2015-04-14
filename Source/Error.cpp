#include "Error.hpp"
#include "Window.hpp"

#include <SDL.h>

extern Window *window;

void Error::report(const std::string &title, const std::string &description)
{
	SDL_Window *parent = nullptr;

	if (window)
		parent = window->getSDLWindow();

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), description.c_str(), parent);
}

const std::string Error::getOpenGLErrorString(GLenum errorFlag)
{
	if (errorFlag == GL_NO_ERROR)
		return "No error has been recorded";
	else if (errorFlag == GL_INVALID_ENUM)
		return "An unacceptable value is specified for an enumerated argument";
	else if (errorFlag == GL_INVALID_VALUE)
		return "A numeric argument is out of range";
	else if (errorFlag == GL_INVALID_OPERATION)
		return "The specified operation is not allowed in the current state";
	else if (errorFlag == GL_INVALID_FRAMEBUFFER_OPERATION)
		return "The framebuffer object is not complete";
	else if (errorFlag == GL_OUT_OF_MEMORY)
		return "There is not enough memory left to execute the command";
	else if (errorFlag == GL_STACK_UNDERFLOW)
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow";
	else if (errorFlag == GL_STACK_OVERFLOW)
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow";
	else
		return "Unrecognized error";
}