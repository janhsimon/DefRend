#include <sstream>

#include "GBufferInspector.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Util\Util.hpp"

extern IRenderer *sceneRenderer;

GBufferInspector::GBufferInspector(glm::vec2 position) : Frame(position, glm::vec2(410.f, 290.f + 90.f))
{

}

GBufferInspector::~GBufferInspector()
{
	/*
	if (testButton)
	{
		testButton->destroy();
		delete testButton;
	}

	if (panelDiffMap)
	{
		panelDiffMap->destroy();
		delete panelDiffMap;
	}

	if (sliderTextureSelect)
	{
		sliderTextureSelect->destroy();
		delete sliderTextureSelect;
	}

	if (labelTextureSelect)
	{
		labelTextureSelect->destroy();
		delete labelTextureSelect;
	}
	*/
}

bool GBufferInspector::create()
{
	if (!load("G-BUFFER INSPECTOR"))
		return false;

	if (!Util::checkMemory(panelDiffMap = new Panel(glm::vec2(5.f, 5.f), glm::vec2(400.f, 225.f), true)))
		return false;

	addChildElement(panelDiffMap);

	if (!Util::checkMemory(sliderTextureSelect = new Slider(glm::vec2(30.f, 240.f), glm::vec2(350.f), 0, 1)))
		return false;

	if (!sliderTextureSelect->load())
		return false;

	addChildElement(sliderTextureSelect);

	if (!Util::checkMemory(labelTextureSelect = new Label(glm::vec2(30.f, 260.f))))
		return false;

	addChildElement(labelTextureSelect);

	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			std::string text = "Albedo";

			if (y == 0 && x == 1)
				text = "Depth";
			else if (y == 1 && x == 0)
				text = "Normals";
			else if (y == 1 && x == 1)
				text = "Specular";

			if (!Util::checkMemory(button[x][y] = new Button(glm::vec2(160.f + x * 110.f, 290.f + y * 50.f), text)))
				return false;

			button[x][y]->create();

			addChildElement(button[x][y]);
		}
	}

	if (!Util::checkMemory(labelMRT0 = new Label(glm::vec2(30.f, 290.f), "MRT 0 (RGB/A):")))
		return false;

	addChildElement(labelMRT0);

	if (!Util::checkMemory(labelMRT0 = new Label(glm::vec2(30.f, 340.f), "MRT 1 (RGB/A):")))
		return false;

	addChildElement(labelMRT0);

	return true;
}

void GBufferInspector::update()
{
	std::stringstream s;
	s << "MRT Index: " << sliderTextureSelect->value;

	if (sliderTextureSelect->value == 0)
		s << " (RGB: Albedo A: Depth)";
	else
		s << " (RGB: Normal A: Specular Intensity)";

	labelTextureSelect->setText(s.str());
	panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[sliderTextureSelect->value];
}