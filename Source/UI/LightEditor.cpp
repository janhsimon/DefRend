#include <sstream>

#include "LightEditor.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Util\Util.hpp"

extern LightManager *lightManager;

LightEditor::LightEditor(glm::vec2 position) : Frame(position, glm::vec2(256.f, 512.f))
{

}

LightEditor::~LightEditor()
{
	/*
	if (panelDiffColorPreview)
	{
		panelDiffColorPreview->destroy();
		delete panelDiffColorPreview;
	}

	if (sliderR)
	{
		sliderR->destroy();
		delete sliderR;
	}

	if (sliderG)
	{
		sliderG->destroy();
		delete sliderG;
	}

	if (sliderB)
	{
		sliderB->destroy();
		delete sliderB;
	}

	if (labelRGB)

	Label *labelRGB;
	Slider *sliderCutoffAngle;
	Label *labelCutoffAngle;
	Slider *sliderDiffuseIntensity;
	Label *labelDiffuseIntensity;
	Slider *sliderSpecularIntensity;
	Label *labelSpecularIntensity;
	Slider *sliderSpecularPower;
	Label *labelSpecularPower;
	*/
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
	std::stringstream s;
	s << "Diffuse Color: " << sliderR->value << "/" << sliderG->value << "/" << sliderB->value;
	labelRGB->setText(s.str());
	glm::vec3 color(sliderR->value / 255.f, sliderG->value / 255.f, sliderB->value / 255.f);
	panelDiffColorPreview->color = glm::vec4(color, 1.f);
	lightManager->spotLights[lightManager->getSelectedPointLight()]->diffuseColor = color;

	s = std::stringstream();
	s << "Diffuse Intensity: " << sliderDiffuseIntensity->value;
	labelDiffuseIntensity->setText(s.str());
	lightManager->spotLights[lightManager->getSelectedPointLight()]->diffuseIntensity = (float)sliderDiffuseIntensity->value;

	s = std::stringstream();
	s << "Specular Intensity: " << sliderSpecularIntensity->value;
	labelSpecularIntensity->setText(s.str());
	lightManager->spotLights[lightManager->getSelectedPointLight()]->specularIntensity = (float)sliderSpecularIntensity->value;

	s = std::stringstream();
	s << "Specular Power: " << sliderSpecularPower->value;
	labelSpecularPower->setText(s.str());
	lightManager->spotLights[lightManager->getSelectedPointLight()]->specularPower = (float)sliderSpecularPower->value;

	s = std::stringstream();
	s << "Cutoff Angle: " << sliderCutoffAngle->value;
	labelCutoffAngle->setText(s.str());
	lightManager->spotLights[lightManager->getSelectedPointLight()]->cutoffAngle = (float)sliderCutoffAngle->value;

	s = std::stringstream();
	float normalizedShadowBias = sliderShadowBias->value / 1000.f;
	s << "Shadow Bias: " << normalizedShadowBias;
	labelShadowBias->setText(s.str());
	lightManager->pointLights[0]->shadowBias = normalizedShadowBias;
}