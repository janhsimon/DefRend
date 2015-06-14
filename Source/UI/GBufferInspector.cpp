#include <sstream>

#include "GBufferInspector.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

extern IRenderer *sceneRenderer;

GBufferInspector::GBufferInspector(glm::vec2 position) : Frame(position, glm::vec2(410.f, 290.f + 90.f))
{

}

GBufferInspector::~GBufferInspector()
{
	delete pushButtonGroup;
}

bool GBufferInspector::create()
{
	if (!load("G-BUFFER INSPECTOR"))
		return false;

	if (!Util::checkMemory(panelDiffMap = new Panel(glm::vec2(5.f, 5.f), glm::vec2(400.f, 225.f), true)))
		return false;

	addChildElement(panelDiffMap);



	if (!Util::checkMemory(labelLayout = new Label(glm::vec2(50.f + 5.f, 240.f + 10.f), "Layout:")))
		return false;

	addChildElement(labelLayout);


	if (!Util::checkMemory(panelRGB = new Panel(glm::vec2(155.f, 240.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelRGB->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelRGB);

	if (!Util::checkMemory(labelRGB = new Label(glm::vec2(155.f + 5.f, 240.f + 10.f), "RGB")))
		return false;

	addChildElement(labelRGB);

	if (!Util::checkMemory(panelA = new Panel(glm::vec2(260.f, 240.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelA->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelA);

	if (!Util::checkMemory(labelA = new Label(glm::vec2(260.f + 5.f, 240.f + 10.f), "A")))
		return false;

	addChildElement(labelA);


	if (!Util::checkMemory(panelMRT0 = new Panel(glm::vec2(50.f, 285.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelMRT0->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelMRT0);

	if (!Util::checkMemory(labelMRT0 = new Label(glm::vec2(50.f + 5.f, 285.f + 10.f), "MRT 0")))
		return false;

	addChildElement(labelMRT0);

	if (!Util::checkMemory(panelMRT1 = new Panel(glm::vec2(50.f, 330.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelMRT1->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelMRT1);

	if (!Util::checkMemory(labelMRT1 = new Label(glm::vec2(50.f + 5.f, 330.f + 10.f), "MRT 1")))
		return false;

	addChildElement(labelMRT1);


	if (!Util::checkMemory(pushButtonGroup = new PushButtonGroup()))
		return false;

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

			bool pushed = false;

			if (x == 0 && y == 0)
				pushed = true;

			if (!Util::checkMemory(pushButton[x][y] = new PushButton(pushed, glm::vec2(155.f + x * 105.f, 285.f + y * 45.f), text)))
				return false;

			pushButton[x][y]->create();
			pushButtonGroup->addPushButton(pushButton[x][y]);

			addChildElement(pushButton[x][y]);
		}
	}

	pushButtonGroup->selectedButton = pushButton[0][0];


	return true;
}

void GBufferInspector::update()
{
	if (pushButton[0][0]->getPushed())
		panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[0];
	else if (pushButton[1][0]->getPushed())
		panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[1];
	/*else if (pushButton[0][1]->getPushed())
		panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[2];
	else if (pushButton[1][1]->getPushed())
		panelDiffMap->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->textures[3];*/
	
	pushButtonGroup->update();
}