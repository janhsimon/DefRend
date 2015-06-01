#pragma once

#include "Element.hpp"
#include "..\Material\Texture.hpp"

class Slider : public Element
{
private:
	Texture *barTexture, *barLeftEndTexture, *barRightEndTexture, *handleTexture, *handleTextureHover;
	glm::vec2 barPositionWorld, handlePositionWorld;
	bool mouseDragging;
	bool hover;

public:
	int min, max, value;

	Slider(glm::vec2 position, glm::vec2 size, int min, int max);
	bool load();
	void destroy();

	void render(UIRenderer *uiRenderer, glm::vec2 parentPosition);

	void onMouseButtonDown(glm::vec2 mousePosition, int mouseButton);
	void onMouseButtonUp(glm::vec2 mousePosition, int mouseButton);
	void onMouseMove(glm::vec2 mousePosition);
};