#pragma once

#include "Element.hpp"
#include "Label.hpp"
#include "..\Material\Texture.hpp"

enum ButtonState
{
	NORMAL,
	HOVER,
	DOWN
};

class Button : public Element
{
private:
	Label *label;
	Texture *texture[3]; // normal, hover, down
	ButtonState state;
	glm::vec2 buttonPositionWorld;
	bool mouseDownOnButton;

public:
	std::string text;
	glm::vec3 color;

	Button(const glm::vec2 & position, std::string text = "");
	bool create();
	void destroy();

	void render(const glm::vec2 &parentPosition);

	void onMouseButtonDown(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 & mousePosition);
};