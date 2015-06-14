#include "PushButton.hpp"

PushButton::PushButton(bool isPushed, const glm::vec2 &position, const std::string &text) : Button(position, text)
{
	setPushed(isPushed);
}

PushButton::~PushButton()
{

}

void PushButton::setPushed(bool isPushed)
{
	this->isPushed = isPushed;

	if (isPushed)
		state = ButtonState::DOWN;
	else
		state = ButtonState::NORMAL;
}

void PushButton::onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton)
{
	if (isPointOnArea(mousePosition, buttonPositionWorld, size))
	{
		state = ButtonState::DOWN;
		mouseDownOnButton = true;
	}
}

void PushButton::onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton)
{
	if (isPointOnArea(mousePosition, buttonPositionWorld, size))
	{
		if (!isPushed)
			state = ButtonState::HOVER;
		else
			state = ButtonState::DOWN;

		if (mouseDownOnButton)
		{
			setPushed(!isPushed);
			group->selectedButton = this;
		}
	}
	else
	{
		if (isPushed)
			state = ButtonState::DOWN;
		else
			state = ButtonState::NORMAL;
	}

	mouseDownOnButton = false;
}

void PushButton::onMouseMove(const glm::vec2 & mousePosition)
{
	if (!isPushed)
	{
		if (state == ButtonState::NORMAL)
		{
			if (isPointOnArea(mousePosition, buttonPositionWorld, size))
				state = ButtonState::HOVER;
		}
		else if (state == ButtonState::HOVER)
		{
			if (!isPointOnArea(mousePosition, buttonPositionWorld, size))
				state = ButtonState::NORMAL;
		}
	}
	else
		state = ButtonState::DOWN;
}