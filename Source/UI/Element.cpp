#include "Element.hpp"

Element::Element(glm::vec2 position, glm::vec2 size)
{
	this->position = position;
	this->size = size;
}

bool Element::isPointOnArea(glm::vec2 point, glm::vec2 position, glm::vec2 size)
{
	return point.x >= position.x && point.x < position.x + size.x && point.y >= position.y && point.y < position.y + size.y;
}

bool Element::isPointOnElement(glm::vec2 point)
{
	return isPointOnArea(point, position, size);
}