#include <gtc\matrix_transform.hpp>

#include "Frame.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern UIRenderer *uiRenderer;
extern Window *window;

Frame::Frame(const glm::vec2 &position, const glm::vec2 &size) : Element(position, size)
{
	mouseDragging = false;
	visible = false;
}

Frame::~Frame()
{
	if (headerTexture)
		TextureManager::unrefTexture(headerTexture);

	if (mainTexture)
		TextureManager::unrefTexture(mainTexture);

	if (titleLabel)
		delete titleLabel;

	for (Element *element : childElements)
		delete element;
}

bool Frame::load(const std::string &title)
{
	headerTexture = TextureManager::refTexture("Textures\\FrameHeader.png");

	if (!headerTexture)
		return false;

	mainTexture = TextureManager::refTexture("Textures\\FrameMain.png");

	if (!mainTexture)
		return false;

	if (!Util::checkMemory(titleLabel = new Label(glm::vec2(12.f, -27.f), title)))
		return false;

	return true;
}

void Frame::addChildElement(Element *element)
{
	assert(element);
	childElements.push_back(element);
}

void Frame::render(const glm::vec2 &parentPosition)
{
	if (!visible)
		return;

	glUseProgram(uiRenderer->getUIDrawShader()->program);


	// render main frame

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + size.y / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, size.y / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);

	mainTexture->bind(GL_TEXTURE0);
	UnitQuad::render();


	// render header frame

	worldMatrix = glm::mat4(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y - 18.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, 32.f / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);

	headerTexture->bind(GL_TEXTURE0);
	UnitQuad::render();


	titleLabel->render(position + parentPosition);

	for (Element *element : childElements)
		element->render(position + parentPosition);
}

void Frame::onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton)
{
	if (!visible)
		return;

	if (isPointOnArea(mousePosition, glm::vec2(position.x, position.y - 36.f), glm::vec2(size.x, 32.f)))
	{
		if (mouseButton == 1)
		{
			lastMousePosition = mousePosition;
			mouseDragging = true;
		}
	}
	else if (isPointOnElement(mousePosition))
	// if the mouse click was over the main area
	{
		for (Element *element : childElements)
			element->onMouseButtonDown(mousePosition, mouseButton);
	}
}

void Frame::onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton)
{
	if (!visible)
		return;

	for (Element *element : childElements)
		element->onMouseButtonUp(mousePosition, mouseButton);

	mouseDragging = false;
}

void Frame::onMouseMove(const glm::vec2 &mousePosition)
{
	if (!visible)
		return;

	for (Element *element : childElements)
		element->onMouseMove(mousePosition);

	if (mouseDragging)
	{
		position += mousePosition - lastMousePosition;
		lastMousePosition = mousePosition;

		if (position.x < 0.f)
			position.x = 0.f;
		else if (position.x >= window->width - size.x)
			position.x = window->width - size.x - 1.f;

		if (position.y < 36.f)
			position.y = 36.f;
		else if (position.y >= window->height - size.y)
			position.y = window->height - size.y - 1.f;
	}
}

void Frame::onMouseWheel(const glm::vec2 &mousePosition, int distance)
{
	if (!visible)
		return;

	for (Element *element : childElements)
		element->onMouseWheel(mousePosition, distance);
}