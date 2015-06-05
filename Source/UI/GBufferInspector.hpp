#pragma once

#include <glm.hpp>

#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"

class GBufferInspector : public Frame
{
private:
	Panel *panelDiffMap;
	Slider *sliderTextureSelect;
	Label *labelTextureSelect;

public:
	GBufferInspector(glm::vec2 position);

	bool create();
	void update();
};