#pragma once

#include <glm.hpp>

#include "..\Renderer\UIRenderer.hpp"

class Element
{
public:
	glm::vec2 position;
	glm::vec2 size;

	Element(glm::vec2 position, glm::vec2 size);

	virtual void destroy() = 0;
	virtual void render(UIRenderer *uiRenderer, glm::vec2 parentPosition) = 0;

	bool isPointOnArea(glm::vec2 point, glm::vec2 position, glm::vec2 size);
	bool isPointOnElement(glm::vec2 point);

	virtual void onMouseButtonDown(glm::vec2 mousePosition, int mouseButton) = 0;
	virtual void onMouseButtonUp(glm::vec2 mousePosition, int mouseButton) = 0;
	virtual void onMouseMove(glm::vec2 mousePosition) = 0;
};