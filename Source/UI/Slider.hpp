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

	Slider(const glm::vec2 &position, const glm::vec2 &size, int min, int max);
	~Slider();

	bool load();

	void render(const glm::vec2 &parentPosition);

	void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 &mousePosition);
	void onMouseWheel(const glm::vec2 &mousePosition, int distance);
};