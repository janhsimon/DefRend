#pragma once

#include <glm.hpp>

#include "PushButton.hpp"
#include "PushButtonGroup.hpp"
#include "Frame.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "Slider.hpp"

class GBufferInspector : public Frame
{
private:
	Panel *panelViewer;
	Label *labelLayout;

	PushButton *buttonGBufferFat, *buttonGBufferSlim, *buttonGBufferSuperSlim;
	PushButtonGroup *buttonGroupGBufferLayout;

	PushButton *fatPushButton[4];
	PushButtonGroup *fatPushButtonGroup;
	Panel *fatPanelRGB, *fatPanelMRT0, *fatPanelMRT1, *fatPanelMRT2, *fatPanelMRT3;
	Label *fatLabelRGB, *fatLabelMRT0, *fatLabelMRT1, *fatLabelMRT2, *fatLabelMRT3;

	PushButton *slimPushButton[2][2];
	PushButtonGroup *slimPushButtonGroup;
	Panel *slimPanelRGB, *slimPanelA, *slimPanelMRT0, *slimPanelMRT1;
	Label *slimLabelRGB, *slimLabelA, *slimLabelMRT0, *slimLabelMRT1;

	PushButton *superSlimPushButton[2][2];
	PushButtonGroup *superSlimPushButtonGroup;
	Panel *superSlimPanelRGB, *superSlimPanelA, *superSlimPanelMRT0, *superSlimPanelMRT1;
	Label *superSlimLabelRGB, *superSlimLabelA, *superSlimLabelMRT0, *superSlimLabelMRT1;

	bool createFatGBufferButtons();
	bool createSlimGBufferButtons();
	bool createSuperSlimGBufferButtons();

public:
	GBufferInspector(glm::vec2 position);
	~GBufferInspector();

	bool create();
	void update();
};