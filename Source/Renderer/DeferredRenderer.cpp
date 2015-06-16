#include <assert.h>
#include <gtc\matrix_transform.hpp>
#include <vector>

#include "DeferredRenderer.hpp"
#include "UnitQuad.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Scene\SceneManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

extern LightManager *lightManager;
extern SceneManager *sceneManager;

DeferredRenderer::~DeferredRenderer()
{
	delete gBuffer;
	delete unitSphereModel;
	delete geometryShader;
	delete directionalLightingShader;
	delete pointLightingShader;
	delete spotLightingShader;
	delete shadowPassShader;
}

bool DeferredRenderer::loadShaders()
{
	if (!Util::checkMemory(geometryShader = new GeometryShader()))
		return false;

	if (!geometryShader->create())
		return false;

	if (!Util::checkMemory(directionalLightingShader = new DirectionalLightingShader()))
		return false;

	if (!directionalLightingShader->create())
		return false;

	if (!Util::checkMemory(pointLightingShader = new PointLightingShader()))
		return false;

	if (!pointLightingShader->create())
		return false;

	if (!Util::checkMemory(spotLightingShader = new SpotLightingShader()))
		return false;

	if (!spotLightingShader->create())
		return false;

	if (!Util::checkMemory(shadowPassShader = new ShadowPassShader()))
		return false;

	if (!shadowPassShader->create())
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

bool DeferredRenderer::init(Window *window)
{
	assert(window);

	if (!Util::checkMemory(gBuffer = new GBuffer()))
		return false;

	if (!gBuffer->load(window->width, window->height))
		return false;

	if (!loadShaders())
		return false;

	if (!loadModels())
		return false;

	this->window = window;

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

void DeferredRenderer::doDirectionalLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(directionalLightingShader->program);

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type != LightType::DIRECTIONAL_LIGHT)
				continue;

		directionalLightingShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
		directionalLightingShader->setLightParameters(l);
		directionalLightingShader->setScreenSizeUniforms(window->width, window->height);
		UnitQuad::render();
	}
}

void DeferredRenderer::doPointLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(pointLightingShader->program);

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type != LightType::POINT_LIGHT)
			continue;

		PointLight *p = (PointLight*)l;

		// bind this light's shadow map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, p->shadowMap->handle);

		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(p->position[0], p->position[1], p->position[2]));
		float s = std::max(p->diffuseIntensity, p->specularIntensity);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		pointLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		pointLightingShader->setScreenSizeUniform(window->width, window->height);
		pointLightingShader->setLightParameters(p);
		pointLightingShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		pointLightingShader->setShadowBiasUniform(p->shadowBias);
		
		unitSphereModel->render(0);

		// unbind shadow map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void DeferredRenderer::doSpotLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(spotLightingShader->program);

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type != LightType::DIRECTIONAL_LIGHT)
			continue;

		SpotLight *s = (SpotLight*)l;

		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(s->position[0], s->position[1], s->position[2]));
		float scale = std::max(s->diffuseIntensity, s->specularIntensity);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(scale));

		spotLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		spotLightingShader->setScreenSizeUniform(window->width, window->height);
		spotLightingShader->setLightParameters(s);
		spotLightingShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));

		unitSphereModel->render(0);
	}

	/*
	if (input.isFlashLightOn())
	{
		flashLight->position[0] = camera->getPosition().x;
		flashLight->position[1] = camera->getPosition().y;
		flashLight->position[2] = camera->getPosition().z;

		flashLight->direction[0] = camera->getForward().x;
		flashLight->direction[1] = camera->getForward().y;
		flashLight->direction[2] = camera->getForward().z;

		// set the world matrix
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(flashLight->position[0], flashLight->position[1], flashLight->position[2]));
		float s = std::max(pointLight->diffuseIntensity, pointLight->specularIntensity);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		spotLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->getViewMatrix(), camera->getProjectionMatrix());
		spotLightingShader->setScreenSizeUniform(window->getWidth(), window->getHeight());
		spotLightingShader->setLightPositionUniform(flashLight->position[0], flashLight->position[1], flashLight->position[2]);
		spotLightingShader->setLightDiffuseUniforms(flashLight->diffuseColor[0], flashLight->diffuseColor[1], flashLight->diffuseColor[2], flashLight->diffuseIntensity);
		spotLightingShader->setLightSpecularUniforms(flashLight->specularIntensity, flashLight->specularPower);
		spotLightingShader->setLightDirectionUniform(flashLight->direction[0], flashLight->direction[1], flashLight->direction[2]);
		spotLightingShader->setLightCutoffAngleUniform(flashLight->cutoffAngle);
		spotLightingShader->setLightAttenuationUniform(flashLight->attenuation[0], flashLight->attenuation[1], flashLight->attenuation[2]);
		spotLightingShader->setEyePositionUniform(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

		unitSphereModel.render(false);
	}
	*/
}

void DeferredRenderer::render(Camera *camera)
{
	// we want to cull backfaces
	glCullFace(GL_BACK);
	

	// SHADOW PASS: draw depth to the shadow maps

	glViewport(0, 0, 1024, 1024);

	for (DirectionalLight *l : lightManager->lights)
	{
		if (l->type != LightType::POINT_LIGHT)
			continue;

		PointLight *p = (PointLight*)l;

		glBindFramebuffer(GL_FRAMEBUFFER, p->shadowMap->FBO);
		glUseProgram(shadowPassShader->program);

		doShadowPass(p);
	}


	// GEOMETRY PASS: draw scene to the geometry buffer

	glViewport(0, 0, window->width, window->height);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(geometryShader->program);
	
	for (Model *model : sceneManager->models)
	{
		geometryShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);
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

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	for (unsigned int i = 0; i < 2; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, gBuffer->textures[i]);
	}

	doDirectionalLightPass(camera);
	doPointLightPass(camera);
	doSpotLightPass(camera);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	for (unsigned int i = 0; i < 2; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}