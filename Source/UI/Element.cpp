#include "Element.hpp"

Element::Element(glm::vec2 position, glm::vec2 size)
{
	this->position = position;
	this->size = size;
}

bool Element::isPointOnElement(glm::vec2 point)
{
	return point.x >= position.x && point.x < position.x + size.x && point.y >= position.y && point.y < position.y + size.y;
}