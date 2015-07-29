#include <sstream>

#include "GBufferInspector.hpp"
#include "..\Camera\Camera.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

extern Camera *camera;
extern IRenderer *sceneRenderer;

GBufferInspector::GBufferInspector(glm::vec2 position) : Frame(position, glm::vec2(820.f, 580.f + 90.f))
{

}

GBufferInspector::~GBufferInspector()
{
	delete pushButtonGroup;
	delete buttonGroupGBufferLayout;
}

bool GBufferInspector::create()
{
	if (!load("G-BUFFER INSPECTOR"))
		return false;

	if (!Util::checkMemory(panelViewer = new Panel(glm::vec2(10.f, 10.f), glm::vec2(800.f, 450.f), true)))
		return false;

	addChildElement(panelViewer);



	if (!Util::checkMemory(labelLayout = new Label(glm::vec2(50.f + 5.f, 475.f + 10.f), "Layout:"))) // 240 -> 475
		return false;

	addChildElement(labelLayout);


	if (!Util::checkMemory(panelRGB = new Panel(glm::vec2(155.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelRGB->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelRGB);

	if (!Util::checkMemory(labelRGB = new Label(glm::vec2(155.f + 5.f, 475.f + 10.f), "RGB")))
		return false;

	addChildElement(labelRGB);

	if (!Util::checkMemory(panelA = new Panel(glm::vec2(260.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	panelA->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelA);

	if (!Util::checkMemory(labelA = new Label(glm::vec2(260.f + 5.f, 475.f + 10.f), "A")))
		return false;

	addChildElement(labelA);


	if (!Util::checkMemory(panelMRT0 = new Panel(glm::vec2(50.f, 520.f), glm::vec2(100.f, 40.f), false))) // 285 -> 520
		return false;

	panelMRT0->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelMRT0);

	if (!Util::checkMemory(labelMRT0 = new Label(glm::vec2(50.f + 5.f, 520.f + 10.f), "MRT 0")))
		return false;

	addChildElement(labelMRT0);

	if (!Util::checkMemory(panelMRT1 = new Panel(glm::vec2(50.f, 565.f), glm::vec2(100.f, 40.f), false))) // 330 -> 565
		return false;

	panelMRT1->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(panelMRT1);

	if (!Util::checkMemory(labelMRT1 = new Label(glm::vec2(50.f + 5.f, 565.f + 10.f), "MRT 1")))
		return false;

	addChildElement(labelMRT1);


	if (!Util::checkMemory(pushButtonGroup = new PushButtonGroup()))
		return false;

	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			std::string text = "Albedo";

			if (x == 1 && y == 0)
				text = "Depth";
			else if (x == 0 && y == 1)
				text = "Normals";
			else if (y == 1 && x == 1)
				text = "Specular";

			bool pushed = false;

			if (x == 0 && y == 0)
				pushed = true;

			if (!Util::checkMemory(pushButton[x][y] = new PushButton(pushed, glm::vec2(155.f + x * 105.f, 520.f + y * 45.f), text)))
				return false;

			pushButton[x][y]->create();
			pushButtonGroup->addPushButton(pushButton[x][y]);
			addChildElement(pushButton[x][y]);
		}
	}

	pushButtonGroup->selectedButton = pushButton[0][0];



	if (!Util::checkMemory(buttonGroupGBufferLayout = new PushButtonGroup()))
		return false;

	if (!Util::checkMemory(buttonGBufferFat = new PushButton(false, glm::vec2(100.f, 620.f), "Fat")))
		return false;

	buttonGBufferFat->create();
	buttonGroupGBufferLayout->addPushButton(buttonGBufferFat);
	addChildElement(buttonGBufferFat);

	if (!Util::checkMemory(buttonGBufferSlim = new PushButton(true, glm::vec2(210.f, 620.f), "Slim")))
		return false;

	buttonGBufferSlim->create();
	buttonGroupGBufferLayout->addPushButton(buttonGBufferSlim);
	addChildElement(buttonGBufferSlim);

	if (!Util::checkMemory(buttonGBufferSuperSlim = new PushButton(false, glm::vec2(320.f, 620.f), "Super Slim")))
		return false;

	buttonGBufferSuperSlim->create();
	buttonGroupGBufferLayout->addPushButton(buttonGBufferSuperSlim);
	addChildElement(buttonGBufferSuperSlim);

	buttonGroupGBufferLayout->selectedButton = buttonGBufferSlim;


	return true;
}

void GBufferInspector::update()
{
	if (sceneRenderer->type != RendererType::DEFERRED_RENDERER)
		return;

	if (pushButton[0][0]->getPushed())
	// albedo button
	{
		panelViewer->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->getTexture(0);
		panelViewer->mode = PanelMode::GBUFFER_DIFFUSE;
	}
	else if (pushButton[1][0]->getPushed())
	// depth button
	{
		panelViewer->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->getTexture(0/*3*/);
		panelViewer->mode = PanelMode::GBUFFER_DEPTH;
	}
	else if (pushButton[0][1]->getPushed())
	// normals button
	{
		panelViewer->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->getTexture(1);
		panelViewer->mode = PanelMode::GBUFFER_NORMAL;
	}
	else if (pushButton[1][1]->getPushed())
	// specular button
	{
		panelViewer->texture = ((DeferredRenderer*)sceneRenderer)->gBuffer->getTexture(1/*2*/);
		panelViewer->mode = PanelMode::GBUFFER_SPECULAR;
	}

	pushButtonGroup->update();



	if (buttonGBufferFat->getPushed())
		((DeferredRenderer*)sceneRenderer)->changeGBufferLayout(GBufferType::FAT);
	else if (buttonGBufferSlim->getPushed())
		((DeferredRenderer*)sceneRenderer)->changeGBufferLayout(GBufferType::SLIM);
	if (buttonGBufferSuperSlim->getPushed())
		((DeferredRenderer*)sceneRenderer)->changeGBufferLayout(GBufferType::SUPER_SLIM);
	
	buttonGroupGBufferLayout->update();
}