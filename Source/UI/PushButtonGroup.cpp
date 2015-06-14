#include "PushButtonGroup.hpp"

void PushButtonGroup::addPushButton(PushButton* pushButton)
{
	pushButton->group = this;
	pushButtons.push_back(pushButton);
}

void PushButtonGroup::update()
{
	for (PushButton *b : pushButtons)
	{
		if (b->getPushed())
			b->setPushed(false);
	}

	selectedButton->setPushed(true);
}