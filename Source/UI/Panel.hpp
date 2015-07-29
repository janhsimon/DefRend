#pragma once

#include <vector>

#include "Element.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Material\Texture.hpp"

enum PanelMode
{
	CUSTOM_TEXTURE,
	SINGLE_COLOR,
	GBUFFER_DIFFUSE,
	GBUFFER_SPECULAR,
	GBUFFER_NORMAL,
	GBUFFER_DEPTH
};

class Panel : public Element
{
private:
	bool isTextured;

public:
	GLuint texture;
	glm::vec4 color;
	//bool isMRTRGB, isMRTA;
	//float mrtScale;
	PanelMode mode;

	Panel(const glm::vec2 &position, const glm::vec2 &size, bool isTextured);
	~Panel();

	// 16-byte alignment for fast SSE math instructions
	void *operator new(size_t size);
	void operator delete(void *p);

	void render(const glm::vec2 &parentPosition = glm::vec2(0.f, 0.f));

	void onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton);
	void onMouseMove(const glm::vec2 &mousePosition);
	void onMouseWheel(const glm::vec2 &mousePosition, int distance);
};