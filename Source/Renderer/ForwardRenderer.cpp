#include <assert.h>
#include <gtc\matrix_transform.hpp>
#include <vector>

#include "ForwardRenderer.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Scene\SceneManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern LightManager *lightManager;
extern SceneManager *sceneManager;
extern Window *window;

ForwardRenderer::~ForwardRenderer()
{
	delete forwardShader;
}

bool ForwardRenderer::loadShaders()
{
	if (!Util::checkMemory(forwardShader = new ForwardShader()))
		return false;

	if (!forwardShader->create())
		return false;

	return true;
}

bool ForwardRenderer::init()
{
	type = RendererType::FORWARD_RENDERER;

	if (!loadShaders())
		return false;

	return true;
}

void ForwardRenderer::render(Camera *camera)
{
	// we want to cull backfaces
	glCullFace(GL_BACK);

	glViewport(0, 0, window->width, window->height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(forwardShader->program);

	for (Model *model : sceneManager->models)
	{
		forwardShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);
		model->render(BindFlag::DIFFUSE_MAP | BindFlag::SPECULAR_MAP | BindFlag::NORMAL_MAP | BindFlag::OPACITY_MAP);
	}

	// switch cull direction for later renderers
	glCullFace(GL_FRONT);
}