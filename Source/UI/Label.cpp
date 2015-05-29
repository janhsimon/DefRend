#include "Label.hpp"

Label::Label(std::string text, glm::vec2 position, glm::vec2 size) : Element(position, size)
{
	this->text = text;

	color = glm::vec3(1.f, 1.f, 1.f);
}

void Label::destroy()
{

}

void Label::render(UIRenderer *uiRenderer, glm::vec2 parentPosition)
{
	SDL_Color sdlColor = { (char)(color.r * 255.f), (char)(color.g * 255.f), (char)(color.b * 255.f) };
	uiRenderer->drawText(text, position + parentPosition, sdlColor);
}

void Label::onMouseButtonDown(glm::vec2 mousePosition, int mouseButton)
{

}

void Label::onMouseButtonUp(glm::vec2 mousePosition, int mouseButton)
{

}

void Label::onMouseMove(glm::vec2 mousePosition)
{

}