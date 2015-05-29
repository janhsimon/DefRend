#include <assert.h>
#include <gtc\matrix_transform.hpp>
#include <vector>

#include "DeferredRenderer.hpp"
#include "UnitQuad.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Scene\SceneManager.hpp"
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

/*
void DeferredRenderer::renderGeometryPass(Camera *camera)
{
	assert(camera);

	//glUseProgram(geometryShader->getProgram());

	//gBuffer->bindForWriting();

	// make sure we write to the depth buffer
	//glDepthMask(GL_TRUE);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);

	for (Model *model : *sceneManager->getModels())
	{
		geometryShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->getViewMatrix(), camera->getProjectionMatrix());
		model->render(true);
	}
}
*/

/*
void DeferredRenderer::renderGBufferDebug()
{
	// reset the frame buffer so we draw to the actual screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	assert(gBuffer);
	gBuffer->bindForReading(true);

	GLsizei halfWidth = (GLsizei)(window->getWidth() / 2.f);
	GLsizei halfHeight = (GLsizei)(window->getHeight() / 2.f);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), 0, halfHeight, halfWidth, window->getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_SPECULAR);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), halfWidth, halfHeight, window->getWidth(), window->getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), halfWidth, 0, window->getWidth(), halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
*/

/*
void DeferredRenderer::doShadowPass(float delta)
{
	glUseProgram(shadowPassShaderProgram->getProgram());

	for (PointLight *pointLight : pointLights)
	{
		glm::mat4 pointLightProjectionMatrix = glm::perspective(glm::radians(90.f), 1.f, 1.0f, calcPointLightBSphere(*pointLight));

		pointLight->beginDrawingToShadowMap();

		for (int i = 0; i < 6; ++i)
		{
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

			glm::mat4 pointLightViewMatrix = glm::lookAt(glm::vec3(pointLight->position[0], pointLight->position[1], pointLight->position[2]), sideDirection, upDirection);

			glUniformMatrix4fv(sp_viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(pointLightProjectionMatrix * pointLightViewMatrix));
			glUniform3f(sp_pointLightPositionLocation, pointLight->position[0], pointLight->position[1], pointLight->position[2]);


			glUniformMatrix4fv(sp_worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
			
			sponzaModel.render(true);


			static float _angle;
			glm::mat4 worldMatrix(1.f);
			worldMatrix = glm::translate(worldMatrix, glm::vec3(-500.f, 0.f, 0.f));
			worldMatrix = glm::rotate(worldMatrix, _angle, glm::vec3(0.f, 1.f, 0.f));
			_angle += delta * .003f;

			glUniformMatrix4fv(sp_worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));

			manModel.render(true);
		}
	}
}
*/

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

float calcPointLightBSphere(const PointLight &l)
{
	float maxChannel = std::max(std::max(l.diffuseColor[0], l.diffuseColor[1]), l.diffuseColor[2]);

	// [0] = constant, [1] = linear, [2] = exponent
	float s = (-l.attenuation[1] + sqrtf(l.attenuation[1] * l.attenuation[1] - 4.f * l.attenuation[2] * (l.attenuation[0] - 256.f * maxChannel * l.diffuseIntensity))) / (2.f * l.attenuation[2]);

	return s;
}

void DeferredRenderer::doPointLightPass(Camera *camera)
{
	assert(camera);

	glUseProgram(pointLightingShader->program);

	for (PointLight *pointLight : lightManager->pointLights)
	{
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(pointLight->position[0], pointLight->position[1], pointLight->position[2]));
		float s = calcPointLightBSphere(*pointLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		pointLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera->viewMatrix, camera->projectionMatrix);
		pointLightingShader->setScreenSizeUniform(window->width, window->height);
		pointLightingShader->setLightParameters(pointLight);
		pointLightingShader->setEyePositionUniform(camera->position.x, camera->position.y, camera->position.z);
		
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
		float s = calcPointLightBSphere(*spotLight);
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
		float s = calcPointLightBSphere(*flashLight);
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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer->FBO);
	glUseProgram(geometryShader->program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Model *model : sceneManager->models)
	{
		geometryShader->setWorldViewProjectionUniforms(model->getWorldMatrix(), camera->viewMatrix, camera->projectionMatrix);
		model->render(true);
	}
		
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < 4; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, gBuffer->textures[i]);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	doDirectionalLightPass(camera);
	doPointLightPass(camera);
	doSpotLightPass(camera);

	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}