#pragma once

#include "Element.hpp"

class Label : public Element
{
private:

public:
	std::string text;
	glm::vec3 color;

	Label(glm::vec2 position, std::string text = "");
	void destroy();

	void render(UIRenderer *uiRenderer, glm::vec2 parentPosition);

	void onMouseButtonDown(glm::vec2 mousePosition, int mouseButton);
	void onMouseButtonUp(glm::vec2 mousePosition, int mouseButton);
	void onMouseMove(glm::vec2 mousePosition);
};