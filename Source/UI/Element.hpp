#pragma once

#include <glm.hpp>

#include "..\Renderer\UIRenderer.hpp"

class Element
{
public:
	glm::vec2 position;
	glm::vec2 size;
	bool visible;

	Element(const glm::vec2 & position, const glm::vec2 & size);
	virtual ~Element();
	//virtual void destroy() = 0;

	virtual void render(const glm::vec2 & parentPosition) = 0;

	bool isPointOnArea(const glm::vec2 &point, const glm::vec2 &position, const glm::vec2 &size);
	bool isPointOnElement(const glm::vec2 &point);

	virtual void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton) = 0;
	virtual void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton) = 0;
	virtual void onMouseWheel(const glm::vec2 &mousePosition, int distance) = 0;
	virtual void onMouseMove(const glm::vec2 &mousePosition) = 0;
};