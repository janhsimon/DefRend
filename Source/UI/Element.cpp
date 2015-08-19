#include "Element.hpp"
#include "..\Util\Error.hpp"

Element::Element(const glm::vec2 &position, const glm::vec2 &size)
{
	this->position = position;
	this->size = size;
	
	visible = true;
}

Element::~Element()
{
	
}

bool Element::isPointOnArea(const glm::vec2 &point, const glm::vec2 &position, const glm::vec2 &size)
{
	if (!visible)
		return false;

	return point.x >= position.x && point.x < position.x + size.x && point.y >= position.y && point.y < position.y + size.y;
}

bool Element::isPointOnElement(const glm::vec2 &point)
{
	if (!visible)
		return false;

	return isPointOnArea(point, position, size);
}