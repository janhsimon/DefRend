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
	
	float fovy = Util::convertHorizontalToVerticalFOV(90.f, (float)window->width, (float)window->height);
	//glm::mat4 pointLightProjectionMatrix = glm::perspective((fovy / 2.f), 1.f, 1.f, std::max(pointLight->diffuseIntensity, pointLight->specularIntensity));
	
	float DEBUGFOV = /*(fovy / 2.f)*/80.f;
	glm::mat4 pointLightProjectionMatrix = glm::perspective(DEBUGFOV, 1.f, 1.f, std::max(pointLight->diffuseIntensity, pointLight->specularIntensity));
	
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
			model->render(false); // no material binds please
		}
	}
}

void DeferredRenderer::doDirectionalLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(directionalLightingShader->program);

	for (DirectionalLight *directionalLight : lightManager->directionalLights)
	{
		directionalLightingShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
		directionalLightingShader->setLightParameters(directionalLight);
		directionalLightingShader->setScreenSizeUniforms(window->width, window->height);
		UnitQuad::render();
	}
}

void DeferredRenderer::doPointLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(pointLightingShader->program);

	for (PointLight *pointLight : lightManager->pointLights)
	{
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(pointLight->position[0], pointLight->position[1], pointLight->position[2]));
		float s = std::max(pointLight->diffuseIntensity, pointLight->specularIntensity);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		pointLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		pointLightingShader->setScreenSizeUniform(window->width, window->height);
		pointLightingShader->setLightParameters(pointLight);
		pointLightingShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		pointLightingShader->setShadowBiasUniform(lightManager->pointLights[0]->shadowBias);
		
		unitSphereModel->render(false);
	}
}

void DeferredRenderer::doSpotLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(spotLightingShader->program);

	for (SpotLight *spotLight : lightManager->spotLights)
	{
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(spotLight->position[0], spotLight->position[1], spotLight->position[2]));
		float s = std::max(spotLight->diffuseIntensity, spotLight->specularIntensity);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));

		spotLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		spotLightingShader->setScreenSizeUniform(window->width, window->height);
		spotLightingShader->setLightParameters(spotLight);
		spotLightingShader->setEyePositionUniform(glm::vec3(camera->position.x, camera->position.y, camera->position.z));

		unitSphereModel->render(false);
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
	// SHADOW PASS: draw depth to the shadow maps

	glViewport(0, 0, 1024, 1024);

	for (PointLight *p : lightManager->pointLights)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, p->shadowMap->FBO);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		model->render(true);
	}
	

	// LIGHTING PASS: draw light primitives to the screen

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

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

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lightManager->pointLights[0]->shadowMap->handle);

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

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}