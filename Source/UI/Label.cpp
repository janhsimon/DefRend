#include "Label.hpp"
#include "..\Renderer\UIRenderer.hpp"

extern UIRenderer *uiRenderer;

Label::Label(const glm::vec2 &position, const std::string &text) : Element(position, glm::vec2())
{
	setText(text);

	color = glm::vec3(1.f, 1.f, 1.f);
}

void Label::destroy()
{

}

void Label::render(const glm::vec2 &parentPosition)
{
	SDL_Color sdlColor = { (char)(color.r * 255.f), (char)(color.g * 255.f), (char)(color.b * 255.f) };
	uiRenderer->drawText(text, position + parentPosition, sdlColor);
}

void Label::setText(const std::string &text)
{
	if (this->text.compare(text) == 0)
		return;

	this->text = text;

	size = uiRenderer->getTextDimensions(text);
}

void Label::onMouseButtonDown(const glm::vec2 & mousePosition, int mouseButton)
{

}

void Label::onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton)
{

}

void Label::onMouseMove(const glm::vec2 & mousePosition)
{

}