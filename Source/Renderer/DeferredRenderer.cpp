#include <assert.h>
#include <gtc\matrix_transform.hpp>
#include <vector>

#include "DeferredRenderer.hpp"
#include "UnitQuad.hpp"
#include "..\GBuffer\GBufferFat.hpp"
#include "..\GBuffer\GBufferSlim.hpp"
#include "..\GBuffer\GBufferSuperSlim.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Scene\SceneManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern LightManager *lightManager;
extern SceneManager *sceneManager;
extern Window *window;

DeferredRenderer::~DeferredRenderer()
{
	delete gBuffer;
	delete unitSphereModel;

	if (geometryFatShader)
		delete geometryFatShader;

	if (deferredFatShader)
		delete deferredFatShader;

	if (geometrySlimShader)
		delete geometrySlimShader;

	if (deferredSlimShader)
		delete deferredSlimShader;

	if (geometrySuperSlimShader)
		delete geometrySuperSlimShader;

	if (deferredSuperSlimShader)
		delete deferredSuperSlimShader;

	if (shadowPassShader)
		delete shadowPassShader;
}

bool DeferredRenderer::loadFatShaders()
{
	if (!Util::checkMemory(geometryFatShader = new GeometryFatShader()))
		return false;

	if (!geometryFatShader->create())
		return false;

	if (!Util::checkMemory(deferredFatShader = new DeferredFatShader()))
		return false;

	if (!deferredFatShader->create())
		return false;

	return true;
}

bool DeferredRenderer::loadSlimShaders()
{
	if (!Util::checkMemory(geometrySlimShader = new GeometrySlimShader()))
		return false;

	if (!geometrySlimShader->create())
		return false;

	if (!Util::checkMemory(deferredSlimShader = new DeferredSlimShader()))
		return false;

	if (!deferredSlimShader->create())
		return false;

	return true;
}

bool DeferredRenderer::loadSuperSlimShaders()
{
	if (!Util::checkMemory(geometrySuperSlimShader = new GeometrySuperSlimShader()))
		return false;

	if (!geometrySuperSlimShader->create())
		return false;

	if (!Util::checkMemory(deferredSuperSlimShader = new DeferredSuperSlimShader()))
		return false;

	if (!deferredSuperSlimShader->create())
		return false;

	return true;
}

bool DeferredRenderer::loadModels()
{
	if (!Util::checkMemory(unitSphereModel = new Model(glm::vec3(0.f))))
		return false;

	if (!unitSphereModel->load("Models\\UnitSphere\\UnitSphere.obj"))
		return false;

	return true;
}

bool DeferredRenderer::init()
{
	type = RendererType::DEFERRED_RENDERER;

	if (!Util::checkMemory(gBuffer = new GBufferSlim()))
		return false;

	if (!gBuffer->load(window->width, window->height))
		return false;

	if (!loadSlimShaders())
		return false;

	geometryFatShader = nullptr;
	deferredFatShader = nullptr;

	geometrySuperSlimShader = nullptr;
	deferredSuperSlimShader = nullptr;

	if (!Util::checkMemory(shadowPassShader = new ShadowPassShader()))
		return false;

	if (!shadowPassShader->create())
		return false;

	if (!loadModels())
		return false;

	doAA = false;

	return true;
}

void DeferredRenderer::doShadowPass(PointLight *pointLight)
{
	assert(pointLight);

	shadowPassShader->setPointLightPositionUniform(pointLight->position);
	shadowPassShader->setCameraFarClipUniform(std::max(pointLight->diffuseIntensity, pointLight->specularIntensity));
	
	// fov needs to be negative 90 to make sure the shadow map sides aren't all flipped upside-down
	glm::mat4 pointLightProjectionMatrix = glm::perspective(glm::radians(-90.f), 1.f, 1.f, std::max(pointLight->diffuseIntensity, pointLight->specularIntensity));
	
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pointLight->shadowMap->handle, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glm::vec3 sideDirection;

		if (i == 0)
			sideDirection = glm::vec3(1.f, 0.f, 0.f);
		else if (i == 1)
			sideDirection = glm::vec3(-1.f, 0.f, 0.f);
		else if (i == 2)
			sideDirection = glm::vec3(0.f, 1.f, 0.f);
		else if (i == 3)
			sideDirection = glm::vec3(0.f, -1.f, 0.f);
		else if (i == 4)
			sideDirection = glm::vec3(0.f, 0.f, 1.f);
		else
			sideDirection = glm::vec3(0.f, 0.f, -1.f);

		glm::vec3 upDirection = glm::vec3(0.f, 1.f, 0.f);

		if (i == 2)
			upDirection = glm::vec3(0.f, 0.f, -1.f);
		else if (i == 3)
			upDirection = glm::vec3(0.f, 0.f, 1.f);

		glm::mat4 pointLightViewMatrix = glm::lookAt(pointLight->position, pointLight->position + sideDirection, upDirection);
			
		for (Model *model : sceneManager->models)
		{
			shadowPassShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), pointLightViewMatrix, pointLightProjectionMatrix);
			model->render(BindFlag::OPACITY_MAP);
		}
	}
}

void DeferredRenderer::render(Camera *camera)
{
	// we want to cull backfaces
	glCullFace(GL_BACK);
	

	// SHADOW PASS: draw depth to the shadow maps of those lights that cast shadows

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type != LightType::POINT_LIGHT)
			continue;

		PointLight *p = (PointLight*)l;

		if (!p->getCastShadows())
			continue;

		unsigned int shadowMapSize = p->shadowMap->size;
		glViewport(0, 0, shadowMapSize, shadowMapSize);

		glBindFramebuffer(GL_FRAMEBUFFER, p->shadowMap->FBO);
		glUseProgram(shadowPassShader->program);

		doShadowPass(p);
	}


	// GEOMETRY PASS: draw scene to the geometry buffer

	glViewport(0, 0, window->width, window->height);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gBuffer->type == GBufferType::FAT)
		glUseProgram(geometryFatShader->program);
	else if (gBuffer->type == GBufferType::SLIM)
		glUseProgram(geometrySlimShader->program);
	else if (gBuffer->type == GBufferType::SUPER_SLIM)
	{
		glUseProgram(geometrySuperSlimShader->program);
		geometrySuperSlimShader->setCameraFarClip(camera->farClipPlane);
	}

	for (Model *model : sceneManager->models)
	{
		if (gBuffer->type == GBufferType::FAT)
			geometryFatShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);
		else if (gBuffer->type == GBufferType::SLIM)
			geometrySlimShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);
		else if (gBuffer->type == GBufferType::SUPER_SLIM)
			geometrySuperSlimShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);

		model->render(BindFlag::DIFFUSE_MAP | BindFlag::SPECULAR_MAP | BindFlag::NORMAL_MAP | BindFlag::OPACITY_MAP);
	}


	// LIGHTING PASS: draw light primitives to the screen

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// now we switch over to culling front faces for light geometry
	glCullFace(GL_FRONT);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glDisable(GL_DEPTH_TEST);

	unsigned int numTextures = 2;
	
	if (gBuffer->type == GBufferType::FAT)
		numTextures = 4;

	for (unsigned int i = 0; i < numTextures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, gBuffer->getTexture(i));
	}

	if (gBuffer->type == GBufferType::FAT)
		glUseProgram(deferredFatShader->program);
	else if (gBuffer->type == GBufferType::SLIM)
		glUseProgram(deferredSlimShader->program);
	else if (gBuffer->type == GBufferType::SUPER_SLIM)
	{
		glUseProgram(deferredSuperSlimShader->program);
		deferredSuperSlimShader->setCameraFarClip(camera->farClipPlane);
	}

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type == LightType::DIRECTIONAL_LIGHT)
		{
			if (gBuffer->type == GBufferType::FAT)
				deferredFatShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
			else if (gBuffer->type == GBufferType::SLIM)
				deferredSlimShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
			else if (gBuffer->type == GBufferType::SUPER_SLIM)
				deferredSuperSlimShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
		}
		else if (l->type == LightType::POINT_LIGHT || l->type == LightType::SPOT_LIGHT)
		{
			PointLight *p = (PointLight*)l;

			if (p->type == LightType::POINT_LIGHT && p->getCastShadows())
			{
				unsigned int shadowMap = GL_TEXTURE2;

				if (gBuffer->type == GBufferType::FAT)
					shadowMap = GL_TEXTURE4;

				// bind this light's shadow map
				glActiveTexture(shadowMap);
				glBindTexture(GL_TEXTURE_CUBE_MAP, p->shadowMap->handle);
			}

			glm::mat4 worldMatrix;
			worldMatrix = glm::translate(worldMatrix, glm::vec3(p->position[0], p->position[1], p->position[2]));
			float scale = std::max(p->diffuseIntensity, p->specularIntensity);
			worldMatrix = glm::scale(worldMatrix, glm::vec3(scale));

			if (gBuffer->type == GBufferType::FAT)
				deferredFatShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
			else if (gBuffer->type == GBufferType::SLIM)
				deferredSlimShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
			else if (gBuffer->type == GBufferType::SUPER_SLIM)
				deferredSuperSlimShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		}
		
		if (gBuffer->type == GBufferType::FAT)
		{
			deferredFatShader->setScreenSizeUniform(window->width, window->height);
			deferredFatShader->setLightParameterUniforms(*l);
			deferredFatShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		}
		else if (gBuffer->type == GBufferType::SLIM)
		{
			deferredSlimShader->setScreenSizeUniform(window->width, window->height);
			deferredSlimShader->setLightParameterUniforms(*l);
			deferredSlimShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		}
		else if (gBuffer->type == GBufferType::SUPER_SLIM)
		{
			deferredSuperSlimShader->setScreenSizeUniform(window->width, window->height);
			deferredSuperSlimShader->setLightParameterUniforms(*l);
			deferredSuperSlimShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		}

		if (l->type == LightType::DIRECTIONAL_LIGHT)
			UnitQuad::render();
		else
			unitSphereModel->render(0);

		if (l->type == LightType::POINT_LIGHT)
		{
			PointLight *p = (PointLight*)l;

			if (p->getCastShadows())
			{
				unsigned int shadowMap = GL_TEXTURE2;

				if (gBuffer->type == GBufferType::FAT)
					shadowMap = GL_TEXTURE4;

				// unbind shadow map
				glActiveTexture(shadowMap);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (unsigned int i = 0; i < numTextures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void DeferredRenderer::changeGBufferLayout(GBufferType type)
{
	assert(window);

	if (gBuffer->type == type)
		return;

	delete gBuffer;

	if (gBuffer->type == GBufferType::FAT)
	{
		delete geometryFatShader;
		delete deferredFatShader;

		geometryFatShader = nullptr;
		deferredFatShader = nullptr;
	}
	else if (gBuffer->type == GBufferType::SLIM)
	{
		delete geometrySlimShader;
		delete deferredSlimShader;

		geometrySlimShader = nullptr;
		deferredSlimShader = nullptr;
	}
	else if (gBuffer->type == GBufferType::SUPER_SLIM)
	{
		delete geometrySuperSlimShader;
		delete deferredSuperSlimShader;

		geometrySuperSlimShader = nullptr;
		deferredSuperSlimShader = nullptr;
	}

	if (type == GBufferType::FAT)
	{
		gBuffer = new GBufferFat();
		loadFatShaders();
	}
	else if (type == GBufferType::SLIM)
	{
		gBuffer = new GBufferSlim();
		loadSlimShaders();
	}
	else if (type == GBufferType::SUPER_SLIM)
	{
		gBuffer = new GBufferSuperSlim();
		loadSuperSlimShaders();
	}

	gBuffer->load(window->width, window->height);
}