#include <glew.h>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

#include "BillboardRenderer.hpp"
#include "UnitQuad.hpp"
#include "..\Light\LightManager.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

const std::string BillboardRenderer::DIRECTIONAL_LIGHT_BILLBOARD_FILENAME = "Textures\\BillboardDirectionalLight.png";
const std::string BillboardRenderer::POINT_LIGHT_BILLBOARD_FILENAME = "Textures\\BillboardPointLight.png";
const std::string BillboardRenderer::SPOT_LIGHT_BILLBOARD_FILENAME = "Textures\\BillboardSpotLight.png";

extern LightManager *lightManager;

bool BillboardRenderer::create()
{
	if (!Util::checkMemory(billboardDrawShader = new BillboardDrawShader()))
		return false;

	if (!billboardDrawShader->create())
		return false;

	if (!Util::checkMemory(lineDrawShader = new LineDrawShader()))
		return false;

	if (!lineDrawShader->create())
		return false;

	if (!Util::checkMemory(directionalLightBillboard = new Texture()))
		return false;

	if (!directionalLightBillboard->load(DIRECTIONAL_LIGHT_BILLBOARD_FILENAME))
		return false;

	if (!Util::checkMemory(pointLightBillboard = new Texture()))
		return false;

	if (!pointLightBillboard->load(POINT_LIGHT_BILLBOARD_FILENAME))
		return false;

	if (!Util::checkMemory(spotLightBillboard = new Texture()))
		return false;

	if (!spotLightBillboard->load(SPOT_LIGHT_BILLBOARD_FILENAME))
		return false;

	if (!Util::checkMemory(unitArrow = new UnitArrow()))
		return false;

	if (!unitArrow->create())
		return false;

	return true;
}

BillboardRenderer::~BillboardRenderer()
{
	delete unitArrow;

	delete directionalLightBillboard;
	delete pointLightBillboard;
	delete spotLightBillboard;

	delete billboardDrawShader;
	delete lineDrawShader;
}

glm::mat4 BillboardRenderer::calculateBillboardMatrix(Camera *camera, DirectionalLight *light)
{
	assert(light);

	glm::mat4 billboardMatrix;

	billboardMatrix[0] = glm::vec4(-camera->getRight(), 0.f);
	billboardMatrix[1] = glm::vec4(camera->getUp(), 0.f);
	billboardMatrix[2] = glm::vec4(glm::normalize(camera->position - light->position), 0.f);
	billboardMatrix[3] = glm::vec4(light->position, 1.f);

	return glm::scale(billboardMatrix, glm::vec3(35.f, 35.f, 35.f));
}

void BillboardRenderer::render(Camera *camera)
{
	for (DirectionalLight *d : lightManager->directionalLights)
	{
		glUseProgram(billboardDrawShader->program);

		billboardDrawShader->setWorldViewProjectionMatrixUniforms(calculateBillboardMatrix(camera, d), camera->viewMatrix, camera->projectionMatrix);
		billboardDrawShader->setTintColorUniform(d->diffuseColor);
		
		directionalLightBillboard->bind(GL_TEXTURE0);
		UnitQuad::render();


		glUseProgram(lineDrawShader->program);

		lineDrawShader->setWorldViewProjectionMatrixUniforms(glm::scale(d->getWorldMatrix(), glm::vec3(100.f, 100.f, 100.f)), camera->viewMatrix, camera->projectionMatrix);
		lineDrawShader->setColorUniform(d->diffuseColor);

		unitArrow->render();
	}

	for (PointLight *l : lightManager->pointLights)
	{
		glUseProgram(billboardDrawShader->program);

		billboardDrawShader->setWorldViewProjectionMatrixUniforms(calculateBillboardMatrix(camera, l), camera->viewMatrix, camera->projectionMatrix);
		
		billboardDrawShader->setTintColorUniform(l->diffuseColor);

		pointLightBillboard->bind(GL_TEXTURE0);
		UnitQuad::render();

		/*
		glUseProgram(lineDrawShader->program);

		lineDrawShader->setWorldViewProjectionMatrixUniforms(glm::scale(l->getWorldMatrix(), glm::vec3(100.f, 100.f, 100.f)), camera->viewMatrix, camera->projectionMatrix);
		lineDrawShader->setColorUniform(glm::vec3(1.f, 0.f, 0.f));

		unitArrow->render();
		*/
	}

	for (unsigned int i = 0; i < lightManager->spotLights.size(); ++i)
	{
		SpotLight *s = lightManager->spotLights[i];

		glUseProgram(billboardDrawShader->program);

		billboardDrawShader->setWorldViewProjectionMatrixUniforms(calculateBillboardMatrix(camera, s), camera->viewMatrix, camera->projectionMatrix);
		
		if (i == lightManager->getSelectedPointLight())
			billboardDrawShader->setTintColorUniform(glm::vec3(1.f, 0.f, 1.f));
		else
			billboardDrawShader->setTintColorUniform(s->diffuseColor);

		spotLightBillboard->bind(GL_TEXTURE0);
		UnitQuad::render();


		glUseProgram(lineDrawShader->program);

		lineDrawShader->setWorldViewProjectionMatrixUniforms(glm::scale(s->getWorldMatrix(), glm::vec3(100.f, 100.f, 100.f)), camera->viewMatrix, camera->projectionMatrix);
		lineDrawShader->setColorUniform(s->diffuseColor);

		unitArrow->render();
	}
}