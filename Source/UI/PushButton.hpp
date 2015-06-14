#pragma once

#include "Button.hpp"
#include "PushButtonGroup.hpp"

class PushButtonGroup;

class PushButton : public Button
{
private:
	bool isPushed;

public:
	PushButtonGroup *group;

	PushButton(bool isPushed, const glm::vec2 &position, const std::string &text);
	~PushButton();

	inline bool getPushed() { return isPushed; }
	void setPushed(bool isPushed);

	void onMouseButtonDown(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 & mousePosition);
	void onMouseWheel(const glm::vec2 &mousePosition, int distance);
};