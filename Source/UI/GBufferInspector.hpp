#pragma once

#include <glm.hpp>

#include "Button.hpp"
#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"

class GBufferInspector : public Frame
{
private:
	Button *button[2][2];
	Panel *panelDiffMap;
	Slider *sliderTextureSelect;
	Label *labelTextureSelect, *labelMRT0, *labelMRT1;

public:
	GBufferInspector(glm::vec2 position);
	~GBufferInspector();

	bool create();
	void update();
};