#include <gtc\matrix_transform.hpp>

#include "Slider.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Error.hpp"
#include "..\Window\Window.hpp"

extern UIRenderer *uiRenderer;
extern Window *window;

Slider::Slider(const glm::vec2 &position, const glm::vec2 &size, int min, int max) : Element(position, size)
{
	this->min = min;
	this->max = max;
	
	value = min + (max - min) / 2;

	mouseDragging = false;
	hover = false;
}

Slider::~Slider()
{
	if (barTexture)
		TextureManager::unrefTexture(barTexture);

	if (barLeftEndTexture)
		TextureManager::unrefTexture(barLeftEndTexture);

	if (barRightEndTexture)
		TextureManager::unrefTexture(barRightEndTexture);

	if (handleTexture)
		TextureManager::unrefTexture(handleTexture);

	if (handleTextureHover)
		TextureManager::unrefTexture(handleTextureHover);
}

bool Slider::load()
{
	barTexture = TextureManager::refTexture("Textures\\SliderBar.png");

	if (!barTexture)
		return false;

	barLeftEndTexture = TextureManager::refTexture("Textures\\SliderBarLeftEnd.png");

	if (!barLeftEndTexture)
		return false;

	barRightEndTexture = TextureManager::refTexture("Textures\\SliderBarRightEnd.png");

	if (!barRightEndTexture)
		return false;

	handleTexture = TextureManager::refTexture("Textures\\SliderHandle.png");

	if (!handleTexture)
		return false;

	handleTextureHover = TextureManager::refTexture("Textures\\SliderHandleHover.png");

	if (!handleTextureHover)
		return false;

	return true;
}

void Slider::render(const glm::vec2 &parentPosition)
{
	glUseProgram(uiRenderer->getUIDrawShader()->program);

	// draw the bar

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + 8.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, 16.f / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(size.x / 16.f, 1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTRGBOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTAOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTScaleUniform(1.f);

	barTexture->bind(GL_TEXTURE0);
	UnitQuad::render();

	barPositionWorld.x = position.x + parentPosition.x;
	barPositionWorld.y = position.y + parentPosition.y;


	// draw the left end

	worldMatrix = glm::mat4(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x - 8.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + 8.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(16.f / window->width, 16.f / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f, 1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTRGBOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTAOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTScaleUniform(1.f);

	barLeftEndTexture->bind(GL_TEXTURE0);
	UnitQuad::render();


	// draw the right end

	worldMatrix = glm::mat4(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x + 8.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + 8.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(16.f / window->width, 16.f / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f, 1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTRGBOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTAOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTScaleUniform(1.f);

	barRightEndTexture->bind(GL_TEXTURE0);
	UnitQuad::render();


	// draw the handle

	float normalizedValue = (float)(value - min) / (max - min);

	worldMatrix = glm::mat4(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x * normalizedValue) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + 9.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(16.f / window->width, 16.f / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	if (mouseDragging || hover)
		handleTextureHover->bind(GL_TEXTURE0);
	else
		handleTexture->bind(GL_TEXTURE0);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f));

	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTRGBOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTAOverrideUniform(false);
	uiRenderer->getUIDrawShader()->setMRTScaleUniform(1.f);

	UnitQuad::render();

	handlePositionWorld.x = position.x + parentPosition.x + size.x * normalizedValue - 8.f;
	handlePositionWorld.y = position.y + parentPosition.y + 1.f;
}

void Slider::onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton)
{
	float normalizedValue = (float)(value - min) / (max - min);

	if (isPointOnArea(mousePosition, glm::vec2(barPositionWorld.x - 8.f, barPositionWorld.y), glm::vec2(size.x + 16.f, 16.f)))
	{
		mouseDragging = true;
		onMouseMove(mousePosition);
	}
}

void Slider::onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton)
{
	mouseDragging = false;
}

void Slider::onMouseMove(const glm::vec2 &mousePosition)
{
	if (isPointOnArea(mousePosition, handlePositionWorld, glm::vec2(16.f, 16.f)))
		hover = true;
	else
		hover = false;

	if (mouseDragging)
	{
		float stepSize = size.x / (max - min);
		value = (int)(((mousePosition.x + stepSize / 2.f - barPositionWorld.x) / size.x) * (max - min));

		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}
}

void Slider::onMouseWheel(const glm::vec2 &mousePosition, int distance)
{
	if (isPointOnArea(mousePosition, glm::vec2(barPositionWorld.x - 8.f, barPositionWorld.y), glm::vec2(size.x + 16.f, 16.f)))
	{
		value += ((max - min) / 50 + 1) * distance;

		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}
}