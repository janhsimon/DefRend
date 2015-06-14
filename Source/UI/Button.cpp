#include <gtc\matrix_transform.hpp>

#include "Button.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern UIRenderer *uiRenderer;
extern Window *window;

Button::Button(const glm::vec2& position, const std::string &text) : Element(position, glm::vec2(100.f, 40.f))
{
	this->text = text;

	color = glm::vec3(1.f, 1.f, 1.f);
	state = ButtonState::NORMAL;
	mouseDownOnButton = false;
}


Button::~Button()
{
	if (label)
		delete label;

	for (int i = 0; i < 3; ++i)
	{
		if (texture[i])
			TextureManager::unrefTexture(texture[i]);
	}
}

bool Button::create()
{
	if (!Util::checkMemory(label = new Label(position, text)))
		return false;

	texture[0] = TextureManager::refTexture("Textures\\ButtonNormal.png");

	if (!texture[0])
		return false;

	texture[1] = TextureManager::refTexture("Textures\\ButtonHover.png");

	if (!texture[1])
		return false;

	texture[2] = TextureManager::refTexture("Textures\\ButtonDown.png");

	if (!texture[2])
		return false;

	return true;
}

void Button::render(const glm::vec2 & parentPosition)
{
	glUseProgram(uiRenderer->getUIDrawShader()->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + size.y / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, size.y / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f));
	uiRenderer->getUIDrawShader()->setColorOverrideUniform(false);

	assert(texture[state]);

	texture[state]->bind(GL_TEXTURE0);
	UnitQuad::render();
	texture[state]->unbind(GL_TEXTURE0);

	assert(label);

	label->color = color;
	label->render(parentPosition + glm::vec2((size.x - label->size.x) / 2.f, (size.y - label->size.y) / 2.f));

	buttonPositionWorld.x = position.x + parentPosition.x;
	buttonPositionWorld.y = position.y + parentPosition.y;
}

void Button::onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton)
{
	if (isPointOnArea(mousePosition, buttonPositionWorld, size))
	{
		state = ButtonState::DOWN;
		mouseDownOnButton = true;
	}
}

void Button::onMouseButtonUp(const glm::vec2 & mousePosition, int mouseButton)
{
	if (isPointOnArea(mousePosition, buttonPositionWorld, size))
	{
		state = ButtonState::HOVER;
		
		if (mouseDownOnButton)
		{
			// trigger
			Error::report("Debug", "Button triggered!");
		}
	}
	else
		state = ButtonState::NORMAL;

	mouseDownOnButton = false;
}

void Button::onMouseMove(const glm::vec2 & mousePosition)
{
	if (state == ButtonState::NORMAL)
	{
		if (isPointOnArea(mousePosition, buttonPositionWorld, size))
			state = ButtonState::HOVER;
	}
	else if (state == ButtonState::HOVER)
	{
		if (!isPointOnArea(mousePosition, buttonPositionWorld, size))
			state = ButtonState::NORMAL;
	}
}

void Button::onMouseWheel(const glm::vec2 &mousePosition, int distance)
{

}