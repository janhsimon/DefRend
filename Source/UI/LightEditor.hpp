#pragma once

#include <glm.hpp>

#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"

class LightEditor : public Frame
{
private:
	Panel *panelDiffColorPreview;
	Slider *sliderR, *sliderG, *sliderB;
	Label *labelRGB;
	Slider *sliderCutoffAngle;
	Label *labelCutoffAngle;
	Slider *sliderDiffuseIntensity;
	Label *labelDiffuseIntensity;
	Slider *sliderSpecularIntensity;
	Label *labelSpecularIntensity;
	Slider *sliderSpecularPower;
	Label *labelSpecularPower;

public:
	LightEditor(glm::vec2 position);

	bool create();
	void update();
};