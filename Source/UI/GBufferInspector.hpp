#pragma once

#include <glm.hpp>

#include "PushButton.hpp"
#include "PushButtonGroup.hpp"
#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"

class GBufferInspector : public Frame
{
private:
	PushButton *pushButton[2][2];
	PushButtonGroup *pushButtonGroup;
	Panel *panelDiffMap, *panelRGB, *panelA, *panelMRT0, *panelMRT1;
	Label *labelLayout, *labelRGB, *labelA, *labelMRT0, *labelMRT1;

public:
	GBufferInspector(glm::vec2 position);
	~GBufferInspector();

	bool create();
	void update();
};