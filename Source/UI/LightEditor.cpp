#include <sstream>

#include "LightEditor.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

extern LightManager *lightManager;

LightEditor::LightEditor(glm::vec2 position) : Frame(position, glm::vec2(256.f, 512.f))
{

}

LightEditor::~LightEditor()
{
	
}

bool LightEditor::create()
{
	if (!load("LIGHT EDITOR"))
		return false;

	if (!Util::checkMemory(sliderR = new Slider(glm::vec2(30.f, 20.f), glm::vec2(150.f), 0, 255)))
		return false;

	if (!sliderR->load())
		return false;

	addChildElement(sliderR);

	if (!Util::checkMemory(sliderG = new Slider(glm::vec2(30.f, 40.f), glm::vec2(150.f), 0, 255)))
		return false;

	if (!sliderG->load())
		return false;

	addChildElement(sliderG);

	if (!Util::checkMemory(sliderB = new Slider(glm::vec2(30.f, 60.f), glm::vec2(150.f), 0, 255)))
		return false;

	if (!sliderB->load())
		return false;

	addChildElement(sliderB);

	if (!Util::checkMemory(panelDiffColorPreview = new Panel(glm::vec2(210.f, 20.f), glm::vec2(30.f, 56.f), false)))
		return false;

	addChildElement(panelDiffColorPreview);

	if (!Util::checkMemory(labelRGB = new Label(glm::vec2(30.f, 80.f))))
		return false;

	addChildElement(labelRGB);


	if (!Util::checkMemory(sliderDiffuseIntensity = new Slider(glm::vec2(30.f, 110.f), glm::vec2(150.f), 0, 2000)))
		return false;

	if (!sliderDiffuseIntensity->load())
		return false;

	addChildElement(sliderDiffuseIntensity);

	if (!Util::checkMemory(labelDiffuseIntensity = new Label(glm::vec2(30.f, 130.f))))
		return false;

	addChildElement(labelDiffuseIntensity);


	if (!Util::checkMemory(sliderSpecularIntensity = new Slider(glm::vec2(30.f, 150.f), glm::vec2(150.f), 0, 2000)))
		return false;

	if (!sliderSpecularIntensity->load())
		return false;

	addChildElement(sliderSpecularIntensity);

	if (!Util::checkMemory(labelSpecularIntensity = new Label(glm::vec2(30.f, 170.f))))
		return false;

	addChildElement(labelSpecularIntensity);


	if (!Util::checkMemory(sliderSpecularPower = new Slider(glm::vec2(30.f, 190.f), glm::vec2(150.f), 1, 64)))
		return false;

	if (!sliderSpecularPower->load())
		return false;

	addChildElement(sliderSpecularPower);

	if (!Util::checkMemory(labelSpecularPower = new Label(glm::vec2(30.f, 210.f))))
		return false;

	addChildElement(labelSpecularPower);


	if (!Util::checkMemory(sliderCutoffAngle = new Slider(glm::vec2(30.f, 240.f), glm::vec2(150.f), 0, 180)))
		return false;

	if (!sliderCutoffAngle->load())
		return false;

	addChildElement(sliderCutoffAngle);

	if (!Util::checkMemory(labelCutoffAngle = new Label(glm::vec2(30.f, 260.f))))
		return false;

	addChildElement(labelCutoffAngle);


	if (!Util::checkMemory(sliderShadowBias = new Slider(glm::vec2(30.f, 290.f), glm::vec2(150.f), 0, 1000)))
		return false;

	if (!sliderShadowBias->load())
		return false;

	addChildElement(sliderShadowBias);

	if (!Util::checkMemory(labelShadowBias = new Label(glm::vec2(30.f, 310.f))))
		return false;

	addChildElement(labelShadowBias);


	return true;
}

void LightEditor::update()
{
	DirectionalLight *selectedDirectionalLight = lightManager->lights[lightManager->getSelectedLightIndex()];

	std::stringstream s;
	s << "Diffuse Color: " << sliderR->value << "/" << sliderG->value << "/" << sliderB->value;
	labelRGB->setText(s.str());
	glm::vec3 color(sliderR->value / 255.f, sliderG->value / 255.f, sliderB->value / 255.f);
	panelDiffColorPreview->color = glm::vec4(color, 1.f);
	selectedDirectionalLight->diffuseColor = color;

	s = std::stringstream();
	s << "Diffuse Intensity: " << sliderDiffuseIntensity->value;
	labelDiffuseIntensity->setText(s.str());
	selectedDirectionalLight->diffuseIntensity = (float)sliderDiffuseIntensity->value;

	if (selectedDirectionalLight->type == LightType::DIRECTIONAL_LIGHT)
		return;

	PointLight *selectedPointLight = (PointLight*)selectedDirectionalLight;

	s = std::stringstream();
	s << "Specular Intensity: " << sliderSpecularIntensity->value;
	labelSpecularIntensity->setText(s.str());
	selectedPointLight->specularIntensity = (float)sliderSpecularIntensity->value;

	s = std::stringstream();
	s << "Specular Power: " << sliderSpecularPower->value;
	labelSpecularPower->setText(s.str());
	selectedPointLight->specularPower = (float)sliderSpecularPower->value;

	if (selectedDirectionalLight->type == LightType::SPOT_LIGHT)
	{
		SpotLight *selectedSpotLight = (SpotLight*)selectedPointLight;

		s = std::stringstream();
		s << "Cutoff Angle: " << sliderCutoffAngle->value;
		labelCutoffAngle->setText(s.str());
		selectedSpotLight->cutoffAngle = (float)sliderCutoffAngle->value;
	}
	else
	{
		s = std::stringstream();
		float normalizedShadowBias = sliderShadowBias->value / 1000.f;
		s << "Shadow Bias: " << normalizedShadowBias;
		labelShadowBias->setText(s.str());
		selectedPointLight->shadowBias = normalizedShadowBias;
	}
}

void LightEditor::loadSliderValuesFromLight(DirectionalLight *l)
{
	sliderR->value = (int)(l->diffuseColor.r * 255.f);
	sliderG->value = (int)(l->diffuseColor.g * 255.f);
	sliderB->value = (int)(l->diffuseColor.b * 255.f);

	sliderDiffuseIntensity->value = (int)(l->diffuseIntensity);
	
	if (l->type == LightType::DIRECTIONAL_LIGHT)
		return;

	PointLight *p = (PointLight*)l;

	sliderSpecularIntensity->value = (int)(p->specularIntensity);
	sliderSpecularPower->value = (int)(p->specularPower);

	if (l->type == LightType::SPOT_LIGHT)
	{
		SpotLight *s = (SpotLight*)p;
		sliderCutoffAngle->value = (int)(s->cutoffAngle);
	}
	else
		sliderShadowBias->value = (int)(p->shadowBias * 1000.f);
}