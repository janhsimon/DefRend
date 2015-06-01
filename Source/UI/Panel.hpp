#pragma once

#include <vector>

#include "Element.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Material\Texture.hpp"

class Panel : public Element
{
public:
	GLuint texture;

	Panel(glm::vec2 position, glm::vec2 size);

	void destroy();

	void render(UIRenderer *uiRenderer, glm::vec2 parentPosition = glm::vec2(0.f, 0.f));

	void onMouseButtonDown(glm::vec2 mousePosition, int mouseButton);
	void onMouseButtonUp(glm::vec2 mousePosition, int mouseButton);
	void onMouseMove(glm::vec2 mousePosition);
};