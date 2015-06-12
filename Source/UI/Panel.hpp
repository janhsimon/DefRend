#pragma once

#include <vector>

#include "Element.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Material\Texture.hpp"

class Panel : public Element
{
private:
	bool isTextured;

public:
	GLuint texture;
	glm::vec4 color;

	Panel(const glm::vec2 &position, const glm::vec2 &size, bool isTextured);

	// 16-byte alignment for fast SSE math instructions
	void *operator new(size_t size);
	void operator delete(void *p);

	void destroy();

	void render(const glm::vec2 &parentPosition = glm::vec2(0.f, 0.f));

	void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 &mousePosition);
};