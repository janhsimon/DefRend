#include <sstream>

#include "GBufferInspector.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Util\Util.hpp"

extern IRenderer *sceneRenderer;

GBufferInspector::GBufferInspector(glm::vec2 position) : Frame(position, glm::vec2(410.f, 290.f))
{

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

	return true;;
}

void GBufferInspector::update()
{
	std::stringstream s;
	s << "MRT Index: " << sliderTextureSelect->value;

	if (sliderTextureSelect->value == 0)
		s << " (RGB: Albedo A: Depth)";
	else
		s << " (RGB: Normal A: Specular Intensity)";

	labelTextureSelect->text = s.str();
	panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[sliderTextureSelect->value];
}