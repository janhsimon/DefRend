#pragma once

#include <vector>

#include "Element.hpp"
#include "Label.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Material\Texture.hpp"

class Frame : public Element
{
private:
	Texture *headerTexture, *mainTexture;
	Label *titleLabel;
	std::vector<Element*> childElements;
	bool mouseDragging;
	glm::vec2 lastMousePosition;

public:
	bool visible;

	Frame(const glm::vec2 &position, const glm::vec2 &size);
	virtual ~Frame();

	bool load(const std::string &title = "");

	void addChildElement(Element *element);
	void render(const glm::vec2 &parentPosition = glm::vec2(0.f, 0.f));

	void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 &mousePosition);
};