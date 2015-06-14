#pragma once

#include <vector>

#include "PushButton.hpp"

class PushButton;

class PushButtonGroup
{
private:
	std::vector<PushButton*> pushButtons;

public:
	PushButton *selectedButton;

	void addPushButton(PushButton* pushButton);
	void update();
};