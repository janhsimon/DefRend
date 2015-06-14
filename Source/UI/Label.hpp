#pragma once

#include "Element.hpp"

class Label : public Element
{
private:
	std::string text;

public:
	glm::vec3 color;

	Label(const glm::vec2 &position, const std::string &text = "");
	~Label();

	//void destroy();

	void render(const glm::vec2 &parentPosition);

	inline std::string &getText() { return text; }
	void setText(const std::string &text);


	void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 & mousePosition);
};