#pragma once

#include <glm.hpp>

#include "Button.hpp"
#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"
#include "..\Light\DirectionalLight.hpp"

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
	Slider *sliderShadowBias;
	Label *labelShadowBias;

	Button *buttonNewDirectionalLight, *buttonNewPointLight, *buttonNewSpotLight, *buttonDeleteLight;

public:
	LightEditor(glm::vec2 position);
	~LightEditor();

	bool create();
	void update();

	void loadSliderValuesFromLight(DirectionalLight *l);
};