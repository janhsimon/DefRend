#include "Element.hpp"

Element::Element(const glm::vec2 &position, const glm::vec2 &size)
{
	this->position = position;
	this->size = size;
}

bool Element::isPointOnArea(const glm::vec2 &point, const glm::vec2 &position, const glm::vec2 &size)
{
	return point.x >= position.x && point.x < position.x + size.x && point.y >= position.y && point.y < position.y + size.y;
}

bool Element::isPointOnElement(const glm::vec2 &point)
{
	return isPointOnArea(point, position, size);
}