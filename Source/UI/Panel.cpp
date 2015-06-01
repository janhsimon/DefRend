#include <gtc\matrix_transform.hpp>

#include "Panel.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern Window *window;

Panel::Panel(glm::vec2 position, glm::vec2 size) : Element(position, size)
{
	
}

void Panel::destroy()
{
	
}

void Panel::render(UIRenderer *uiRenderer, glm::vec2 parentPosition)
{
	glUseProgram(uiRenderer->getUIDrawShader()->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + size.y / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, size.y / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f));

	//texture->bind(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	UnitQuad::render();
}

void Panel::onMouseButtonDown(glm::vec2 mousePosition, int mouseButton)
{
	
}

void Panel::onMouseButtonUp(glm::vec2 mousePosition, int mouseButton)
{
	
}

void Panel::onMouseMove(glm::vec2 mousePosition)
{
	
}