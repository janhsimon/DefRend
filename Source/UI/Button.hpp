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

protected:
	ButtonState state;
	glm::vec2 buttonPositionWorld;
	bool mouseDownOnButton;

public:
	std::string text;
	glm::vec3 color;
	void (*onClick)();

	Button(const glm::vec2 &position, const std::string &text = "");
	~Button();

	bool create();

	void render(const glm::vec2 &parentPosition);

	void onMouseButtonDown(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 & mousePosition);
	void onMouseWheel(const glm::vec2 &mousePosition, int distance);
};