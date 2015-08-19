#include <gtc\matrix_transform.hpp>

#include "Panel.hpp"
#include "..\Camera\Camera.hpp"
#include "..\GBuffer\IGBuffer.hpp"
#include "..\Material\TextureManager.hpp"
#include "..\Renderer\DeferredRenderer.hpp"
#include "..\Renderer\IRenderer.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Renderer\UIRenderer.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern Camera *camera;
extern IRenderer *sceneRenderer;
extern UIRenderer *uiRenderer;
extern Window *window;

Panel::Panel(const glm::vec2 &position, const glm::vec2 &size, bool isTextured) : Element(position, size)
{
	this->isTextured = isTextured;

	//isMRTRGB = false;
	//isMRTA = false;
	//mrtScale = 1.f;

	mode = PanelMode::CUSTOM_TEXTURE;
}

Panel::~Panel()
{

}

void *Panel::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void Panel::operator delete(void *p)
{
	_aligned_free(p);
}

void Panel::render(const glm::vec2 &parentPosition)
{
	glUseProgram(uiRenderer->getUIDrawShader()->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + parentPosition.x + size.x / 2.f) / window->width) * 2.f - 1.f, -((position.y + parentPosition.y + size.y / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(size.x / window->width, size.y / window->height, 1.f));
	uiRenderer->getUIDrawShader()->setWorldMatrixUniform(worldMatrix);

	// flip texture vertically which is needed for some reason
	uiRenderer->getUIDrawShader()->setUVScaleUniform(glm::vec2(1.f, -1.f));

	uiRenderer->getUIDrawShader()->setColorUniform(color);
	uiRenderer->getUIDrawShader()->setModeUniform(!isTextured ? 1 : mode);
	uiRenderer->getUIDrawShader()->setCameraFarClip(camera->farClipPlane);

	if (isTextured)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	if (sceneRenderer->type == DEFERRED_RENDERER)
	{
		DeferredRenderer *deferredRenderer = (DeferredRenderer*)sceneRenderer;
		uiRenderer->getUIDrawShader()->setGBufferLayoutUniform(deferredRenderer->gBuffer->type);

		if (deferredRenderer->gBuffer->type == GBufferType::SUPER_SLIM)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
	}
	
	UnitQuad::render();

	if (isTextured)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Panel::onMouseButtonDown(const glm::vec2 &mousePosition, int mouseButton)
{
	
}

void Panel::onMouseButtonUp(const glm::vec2 &mousePosition, int mouseButton)
{
	
}

void Panel::onMouseMove(const glm::vec2 &mousePosition)
{
	
}

void Panel::onMouseWheel(const glm::vec2 &mousePosition, int distance)
{

}