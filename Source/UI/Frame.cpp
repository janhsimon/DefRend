#include <gtc\matrix_transform.hpp>

#include "Frame.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern Window *window;

Frame::Frame(glm::vec2 position, glm::vec2 size) : Element(position, size)
{
	mouseDragging = false;
}

bool Frame::load()
{
	backgroundTexture = TextureManager::refTexture("Textures\\Frame.png");

	if (!backgroundTexture)
		return false;

	return true;
}

void Frame::destroy()
{
	if (backgroundTexture)
		TextureManager::unrefTexture(backgroundTexture);

	for (Element *element : childElements)
	{
		element->destroy();
		delete element;
	}
}

void Frame::addChildElement(Element *element)
{
	assert(element);
	childElements.push_back(element);
}

void Frame::render(UIRenderer *uiRenderer, glm::vec2 parentPosition)
{
	glUseProgram(uiRenderer->getUIDrawShader()->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + size.y / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, size.y / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	backgroundTexture->bind(GL_TEXTURE0);
	UnitQuad::render();

	for (Element *element : childElements)
		element->render(uiRenderer, position + parentPosition);
}

void Frame::onMouseButtonDown(glm::vec2 mousePosition, int mouseButton)
{
	if (mouseButton == 1 && isPointOnElement(mousePosition))
	{
		lastMousePosition = mousePosition;
		mouseDragging = true;
	}
}

void Frame::onMouseButtonUp(glm::vec2 mousePosition, int mouseButton)
{
	mouseDragging = false;
}

void Frame::onMouseMove(glm::vec2 mousePosition)
{
	if (mouseDragging)
	{
		position += mousePosition - lastMousePosition;
		lastMousePosition = mousePosition;

		if (position.x < 0.f)
			position.x = 0.f;
		else if (position.x >= window->width - size.x)
			position.x = window->width - size.x - 1.f;

		if (position.y < 0.f)
			position.y = 0.f;
		else if (position.y >= window->height - size.y)
			position.y = window->height - size.y - 1.f;
	}
}