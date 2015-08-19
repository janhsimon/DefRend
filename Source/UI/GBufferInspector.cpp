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
	delete fatPushButtonGroup;
	delete slimPushButtonGroup;
	delete superSlimPushButtonGroup;

	delete buttonGroupGBufferLayout;
}

bool GBufferInspector::createFatGBufferButtons()
{
	if (!Util::checkMemory(fatPanelRGB = new Panel(glm::vec2(50.f, 520.f), glm::vec2(100.f, 40.f), false)))
		return false;

	fatPanelRGB->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	fatPanelRGB->visible = false;
	addChildElement(fatPanelRGB);

	if (!Util::checkMemory(fatLabelRGB = new Label(glm::vec2(50.f + 5.f, 520.f + 10.f), "RGB")))
		return false;

	fatLabelRGB->visible = false;
	addChildElement(fatLabelRGB);


	if (!Util::checkMemory(fatPanelMRT0 = new Panel(glm::vec2(155.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	fatPanelMRT0->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	fatPanelMRT0->visible = false;
	addChildElement(fatPanelMRT0);

	if (!Util::checkMemory(fatLabelMRT0 = new Label(glm::vec2(155.f + 5.f, 475.f + 10.f), "MRT 0")))
		return false;

	fatLabelMRT0->visible = false;
	addChildElement(fatLabelMRT0);

	if (!Util::checkMemory(fatPanelMRT1 = new Panel(glm::vec2(260.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	fatPanelMRT1->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	fatPanelMRT1->visible = false;
	addChildElement(fatPanelMRT1);

	if (!Util::checkMemory(fatLabelMRT1 = new Label(glm::vec2(260.f + 5.f, 475.f + 10.f), "MRT 1")))
		return false;

	fatLabelMRT1->visible = false;
	addChildElement(fatLabelMRT1);

	if (!Util::checkMemory(fatPanelMRT2 = new Panel(glm::vec2(365.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	fatPanelMRT2->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	fatPanelMRT2->visible = false;
	addChildElement(fatPanelMRT2);

	if (!Util::checkMemory(fatLabelMRT2 = new Label(glm::vec2(365.f + 5.f, 475.f + 10.f), "MRT 2")))
		return false;

	fatLabelMRT2->visible = false;
	addChildElement(fatLabelMRT2);

	if (!Util::checkMemory(fatPanelMRT3 = new Panel(glm::vec2(470.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	fatPanelMRT3->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	fatPanelMRT3->visible = false;
	addChildElement(fatPanelMRT3);

	if (!Util::checkMemory(fatLabelMRT3 = new Label(glm::vec2(470.f + 5.f, 475.f + 10.f), "MRT 3")))
		return false;

	fatLabelMRT3->visible = false;
	addChildElement(fatLabelMRT3);


	if (!Util::checkMemory(fatPushButtonGroup = new PushButtonGroup()))
		return false;

	for (int x = 0; x < 4; ++x)
	{
		std::string text = "Albedo";

		if (x == 1)
			text = "Normals";
		else if (x == 2)
			text = "Specular";
		else if (x == 3)
			text = "Position";

		bool pushed = false;

		if (x == 0)
			pushed = true;

		if (!Util::checkMemory(fatPushButton[x] = new PushButton(pushed, glm::vec2(155.f + x * 105.f, 520.f), text)))
			return false;

		fatPushButton[x]->create();
		fatPushButtonGroup->addPushButton(fatPushButton[x]);
		fatPushButton[x]->visible = false;
		addChildElement(fatPushButton[x]);
	}

	fatPushButtonGroup->selectedButton = fatPushButton[0];

	return true;
}

bool GBufferInspector::createSlimGBufferButtons()
{
	if (!Util::checkMemory(slimPanelRGB = new Panel(glm::vec2(155.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	slimPanelRGB->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(slimPanelRGB);

	if (!Util::checkMemory(slimLabelRGB = new Label(glm::vec2(155.f + 5.f, 475.f + 10.f), "RGB")))
		return false;

	addChildElement(slimLabelRGB);

	if (!Util::checkMemory(slimPanelA = new Panel(glm::vec2(260.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	slimPanelA->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(slimPanelA);

	if (!Util::checkMemory(slimLabelA = new Label(glm::vec2(260.f + 5.f, 475.f + 10.f), "A")))
		return false;

	addChildElement(slimLabelA);


	if (!Util::checkMemory(slimPanelMRT0 = new Panel(glm::vec2(50.f, 520.f), glm::vec2(100.f, 40.f), false)))
		return false;

	slimPanelMRT0->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(slimPanelMRT0);

	if (!Util::checkMemory(slimLabelMRT0 = new Label(glm::vec2(50.f + 5.f, 520.f + 10.f), "MRT 0")))
		return false;

	addChildElement(slimLabelMRT0);

	if (!Util::checkMemory(slimPanelMRT1 = new Panel(glm::vec2(50.f, 565.f), glm::vec2(100.f, 40.f), false)))
		return false;

	slimPanelMRT1->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	addChildElement(slimPanelMRT1);

	if (!Util::checkMemory(slimLabelMRT1 = new Label(glm::vec2(50.f + 5.f, 565.f + 10.f), "MRT 1")))
		return false;

	addChildElement(slimLabelMRT1);


	if (!Util::checkMemory(slimPushButtonGroup = new PushButtonGroup()))
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

			if (!Util::checkMemory(slimPushButton[x][y] = new PushButton(pushed, glm::vec2(155.f + x * 105.f, 520.f + y * 45.f), text)))
				return false;

			slimPushButton[x][y]->create();
			slimPushButtonGroup->addPushButton(slimPushButton[x][y]);
			addChildElement(slimPushButton[x][y]);
		}
	}

	slimPushButtonGroup->selectedButton = slimPushButton[0][0];

	return true;
}

bool GBufferInspector::createSuperSlimGBufferButtons()
{
	if (!Util::checkMemory(superSlimPanelRGB = new Panel(glm::vec2(155.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	superSlimPanelRGB->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	superSlimPanelRGB->visible = false;
	addChildElement(superSlimPanelRGB);

	if (!Util::checkMemory(superSlimLabelRGB = new Label(glm::vec2(155.f + 5.f, 475.f + 10.f), "RGB")))
		return false;

	superSlimLabelRGB->visible = false;
	addChildElement(superSlimLabelRGB);

	if (!Util::checkMemory(superSlimPanelA = new Panel(glm::vec2(260.f, 475.f), glm::vec2(100.f, 40.f), false)))
		return false;

	superSlimPanelA->color = glm::vec4(0.f, 0.f, 0.f, .5f);
	
	superSlimPanelA->visible = false;
	addChildElement(superSlimPanelA);

	if (!Util::checkMemory(superSlimLabelA = new Label(glm::vec2(260.f + 5.f, 475.f + 10.f), "A")))
		return false;

	superSlimLabelA->visible = false;
	addChildElement(superSlimLabelA);


	if (!Util::checkMemory(superSlimPanelMRT0 = new Panel(glm::vec2(50.f, 520.f), glm::vec2(100.f, 40.f), false)))
		return false;

	superSlimPanelMRT0->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	superSlimPanelMRT0->visible = false;
	addChildElement(superSlimPanelMRT0);

	if (!Util::checkMemory(superSlimLabelMRT0 = new Label(glm::vec2(50.f + 5.f, 520.f + 10.f), "MRT 0")))
		return false;

	superSlimLabelMRT0->visible = false;
	addChildElement(superSlimLabelMRT0);

	if (!Util::checkMemory(superSlimPanelMRT1 = new Panel(glm::vec2(50.f, 565.f), glm::vec2(100.f, 40.f), false)))
		return false;

	superSlimPanelMRT1->color = glm::vec4(0.f, 0.f, 0.f, .5f);

	superSlimPanelMRT1->visible = false;
	addChildElement(superSlimPanelMRT1);

	if (!Util::checkMemory(superSlimLabelMRT1 = new Label(glm::vec2(50.f + 5.f, 565.f + 10.f), "MRT 1")))
		return false;

	superSlimLabelMRT1->visible = false;
	addChildElement(superSlimLabelMRT1);


	if (!Util::checkMemory(superSlimPushButtonGroup = new PushButtonGroup()))
		return false;

	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			std::string text = "Albedo";

			if (x == 1 && y == 0)
				text = "Specular";
			else if (x == 0 && y == 1)
				text = "Normals (RG)";
			else if (y == 1 && x == 1)
				text = "Depth (BA)";

			bool pushed = false;

			if (x == 0 && y == 0)
				pushed = true;

			if (!Util::checkMemory(superSlimPushButton[x][y] = new PushButton(pushed, glm::vec2(155.f + x * 105.f, 520.f + y * 45.f), text)))
				return false;

			superSlimPushButton[x][y]->create();
			superSlimPushButtonGroup->addPushButton(superSlimPushButton[x][y]);
			superSlimPushButton[x][y]->visible = false;
			addChildElement(superSlimPushButton[x][y]);
		}
	}

	superSlimPushButtonGroup->selectedButton = superSlimPushButton[0][0];

	return true;
}

bool GBufferInspector::create()
{
	if (!load("G-BUFFER INSPECTOR"))
		return false;

	if (!Util::checkMemory(panelViewer = new Panel(glm::vec2(10.f, 10.f), glm::vec2(800.f, 450.f), true)))
		return false;

	addChildElement(panelViewer);



	if (!Util::checkMemory(labelLayout = new Label(glm::vec2(50.f + 5.f, 475.f + 10.f), "Layout:")))
		return false;

	addChildElement(labelLayout);



	createFatGBufferButtons();
	createSlimGBufferButtons();
	createSuperSlimGBufferButtons();
	


	if (!Util::checkMemory(buttonGroupGBufferLayout = new PushButtonGroup()))
		return false;

	if (!Util::checkMemory(buttonGBufferFat = new PushButton(false, glm::vec2(50.f, 610.f), "Fat")))
		return false;

	buttonGBufferFat->create();
	buttonGroupGBufferLayout->addPushButton(buttonGBufferFat);
	addChildElement(buttonGBufferFat);

	if (!Util::checkMemory(buttonGBufferSlim = new PushButton(true, glm::vec2(155.f, 610.f), "Slim")))
		return false;

	buttonGBufferSlim->create();
	buttonGroupGBufferLayout->addPushButton(buttonGBufferSlim);
	addChildElement(buttonGBufferSlim);

	if (!Util::checkMemory(buttonGBufferSuperSlim = new PushButton(false, glm::vec2(260.f, 610.f), "Super Slim")))
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

	DeferredRenderer *deferredRenderer = (DeferredRenderer*)sceneRenderer;

	if (buttonGBufferFat->getPushed())
	{
		deferredRenderer->changeGBufferLayout(GBufferType::FAT);

		for (int i = 0; i < 4; ++i)
			fatPushButton[i]->visible = true;

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				slimPushButton[x][y]->visible = false;

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				superSlimPushButton[x][y]->visible = false;

		fatPanelRGB->visible = true;
		fatPanelMRT0->visible = true;
		fatPanelMRT1->visible = true;
		fatPanelMRT2->visible = true;
		fatPanelMRT3->visible = true;
		fatLabelRGB->visible = true;
		fatLabelMRT0->visible = true;
		fatLabelMRT1->visible = true;
		fatLabelMRT2->visible = true;
		fatLabelMRT3->visible = true;

		slimPanelRGB->visible = false;
		slimPanelA->visible = false;
		slimPanelMRT0->visible = false;
		slimPanelMRT1->visible = false;
		slimLabelRGB->visible = false;
		slimLabelA->visible = false;
		slimLabelMRT0->visible = false;
		slimLabelMRT1->visible = false;

		superSlimPanelRGB->visible = false;
		superSlimPanelA->visible = false;
		superSlimPanelMRT0->visible = false;
		superSlimPanelMRT1->visible = false;
		superSlimLabelRGB->visible = false;
		superSlimLabelA->visible = false;
		superSlimLabelMRT0->visible = false;
		superSlimLabelMRT1->visible = false;

		if (fatPushButton[0]->getPushed())
		// albedo button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(0);
			panelViewer->mode = PanelMode::GBUFFER_DIFFUSE;
		}
		else if (fatPushButton[1]->getPushed())
		// normals button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(1);
			panelViewer->mode = PanelMode::GBUFFER_NORMAL;
		}
		else if (fatPushButton[2]->getPushed())
		// specular button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(2);
			panelViewer->mode = PanelMode::GBUFFER_SPECULAR;
		}
		else if (fatPushButton[3]->getPushed())
		// position button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(3);
			panelViewer->mode = PanelMode::GBUFFER_DEPTH;
		}

		fatPushButtonGroup->update();
	}
	else if (buttonGBufferSlim->getPushed())
	{
		deferredRenderer->changeGBufferLayout(GBufferType::SLIM);

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				slimPushButton[x][y]->visible = true;

		for (int i = 0; i < 4; ++i)
			fatPushButton[i]->visible = false;

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				superSlimPushButton[x][y]->visible = false;

		slimPanelRGB->visible = true;
		slimPanelA->visible = true;
		slimPanelMRT0->visible = true;
		slimPanelMRT1->visible = true;
		slimLabelRGB->visible = true;
		slimLabelA->visible = true;
		slimLabelMRT0->visible = true;
		slimLabelMRT1->visible = true;

		fatPanelRGB->visible = false;
		fatPanelMRT0->visible = false;
		fatPanelMRT1->visible = false;
		fatPanelMRT2->visible = false;
		fatPanelMRT3->visible = false;
		fatLabelRGB->visible = false;
		fatLabelMRT0->visible = false;
		fatLabelMRT1->visible = false;
		fatLabelMRT2->visible = false;
		fatLabelMRT3->visible = false;

		superSlimPanelRGB->visible = false;
		superSlimPanelA->visible = false;
		superSlimPanelMRT0->visible = false;
		superSlimPanelMRT1->visible = false;
		superSlimLabelRGB->visible = false;
		superSlimLabelA->visible = false;
		superSlimLabelMRT0->visible = false;
		superSlimLabelMRT1->visible = false;

		if (slimPushButton[0][0]->getPushed())
		// albedo button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(0);
			panelViewer->mode = PanelMode::GBUFFER_DIFFUSE;
		}
		else if (slimPushButton[1][0]->getPushed())
		// depth button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(0);
			panelViewer->mode = PanelMode::GBUFFER_DEPTH;
		}
		else if (slimPushButton[0][1]->getPushed())
		// normals button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(1);
			panelViewer->mode = PanelMode::GBUFFER_NORMAL;
		}
		else if (slimPushButton[1][1]->getPushed())
		// specular button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(1);
			panelViewer->mode = PanelMode::GBUFFER_SPECULAR;
		}

		slimPushButtonGroup->update();
	}
	if (buttonGBufferSuperSlim->getPushed())
	{
		deferredRenderer->changeGBufferLayout(GBufferType::SUPER_SLIM);

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				superSlimPushButton[x][y]->visible = true;

		for (int i = 0; i < 4; ++i)
			fatPushButton[i]->visible = false;

		for (int y = 0; y < 2; ++y)
			for (int x = 0; x < 2; ++x)
				slimPushButton[x][y]->visible = false;

		superSlimPanelRGB->visible = true;
		superSlimPanelA->visible = true;
		superSlimPanelMRT0->visible = true;
		superSlimPanelMRT1->visible = true;
		superSlimLabelRGB->visible = true;
		superSlimLabelA->visible = true;
		superSlimLabelMRT0->visible = true;
		superSlimLabelMRT1->visible = true;

		slimPanelRGB->visible = false;
		slimPanelA->visible = false;
		slimPanelMRT0->visible = false;
		slimPanelMRT1->visible = false;
		slimLabelRGB->visible = false;
		slimLabelA->visible = false;
		slimLabelMRT0->visible = false;
		slimLabelMRT1->visible = false;

		fatPanelRGB->visible = false;
		fatPanelMRT0->visible = false;
		fatPanelMRT1->visible = false;
		fatPanelMRT2->visible = false;
		fatPanelMRT3->visible = false;
		fatLabelRGB->visible = false;
		fatLabelMRT0->visible = false;
		fatLabelMRT1->visible = false;
		fatLabelMRT2->visible = false;
		fatLabelMRT3->visible = false;

		if (superSlimPushButton[0][0]->getPushed())
		// albedo button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(0);
			panelViewer->mode = PanelMode::GBUFFER_DIFFUSE;
		}
		else if (superSlimPushButton[1][0]->getPushed())
		// specular button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(0);
			panelViewer->mode = PanelMode::GBUFFER_SPECULAR;
		}
		else if (superSlimPushButton[0][1]->getPushed())
		// normals button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(1);
			panelViewer->mode = PanelMode::GBUFFER_NORMAL;
		}
		else if (superSlimPushButton[1][1]->getPushed())
		// depth button
		{
			panelViewer->texture = deferredRenderer->gBuffer->getTexture(1);
			panelViewer->mode = PanelMode::GBUFFER_DEPTH;
		}

		superSlimPushButtonGroup->update();
	}
	
	buttonGroupGBufferLayout->update();
}