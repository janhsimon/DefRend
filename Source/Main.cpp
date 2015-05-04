#include "Camera.hpp"
#include "DirectionalLightingShader.hpp"
#include "Error.hpp"
#include "GBuffer.hpp"
#include "GeometryShader.hpp"
#include "Input.hpp"
#include "Model.hpp"
#include "PointLight.hpp"
#include "PointLightingShader.hpp"
#include "Renderer.hpp"
#include "SpotLight.hpp"
#include "SpotLightingShader.hpp"
#include "TextureManager.hpp"
#include "Util.hpp"
#include "Window.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
#include <SDL_opengl.h>

Camera camera;
GBuffer *gBuffer;
Input input;
Model fullscreenQuadModel, sponzaModel, manModel, unitSphereModel;
Renderer *renderer;
//ShaderProgram *shadowPassShaderProgram;
Window *window;

GeometryShader *geometryShader;
DirectionalLightingShader *directionalLightingShader;
PointLightingShader *pointLightingShader;
SpotLightingShader *spotLightingShader;

std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;
SpotLight *flashLight;

unsigned int lastTickTime = 0;

glm::mat4 projectionMatrix;

//GLint sp_worldMatrixLocation, sp_viewProjectionMatrixLocation, sp_pointLightPositionLocation;

/*
bool initShadowPassShaderProgram()
{
	shadowPassShaderProgram = new ShaderProgram();

	assert(shadowPassShaderProgram);

	if (!shadowPassShaderProgram->load("Shaders\\ShadowPass.vs.glsl", "", "Shaders\\ShadowPass.fs.glsl"))
		return false;

	if (!shadowPassShaderProgram->link())
		return false;

	glUseProgram(shadowPassShaderProgram->getProgram());

	if (!shadowPassShaderProgram->getUniformLocation("worldMatrix", sp_worldMatrixLocation))
		return false;

	if (!shadowPassShaderProgram->getUniformLocation("viewProjectionMatrix", sp_viewProjectionMatrixLocation))
		return false;

	if (!shadowPassShaderProgram->getUniformLocation("pointLightPosition", sp_pointLightPositionLocation))
		return false;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load shadow shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}
*/

bool load()
{
	// create a window

	if (!Util::checkMemory(window = new Window()))
		return false;

	if (!window->create(1600, 900))
		return false;


	// create a renderer

	if (!Util::checkMemory(renderer = new Renderer()))
		return false;

	if (!renderer->create())
		return false;


	// check there was no error initializing OpenGL

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to initialize OpenGL: " + Error::getOpenGLErrorString(error));
		return false;
	}


	// initialize glew

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		Error::report("Error", "Failed to initialize GLEW.");
		return false;
	}

	if (!GLEW_VERSION_4_4)
	{
		Error::report("Error", "OpenGL v4.4 is required but your system does not support it.");
		return false;
	}

	error = glGetError();
	if (error != GL_NO_ERROR && error != GL_INVALID_ENUM)
	// ignore a possible invalid enum error after GLEW initialization, known OpenGL issue without fix :(
	{
		Error::report("Error", "Failed to initialize GLEW: " + Error::getOpenGLErrorString(error));
		return false;
	}


	// load shaders
	
	//if (!initShadowPassShaderProgram())
		//return false;

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


	// load models

	if (!fullscreenQuadModel.load("Models\\FullscreenQuad\\FullscreenQuad.obj"))
		return false;

	if (!sponzaModel.load("Models\\Sponza\\Sponza.obj"))
		return false;

	if (!manModel.load("Models\\OldMan\\OldMan.obj"))
		return false;

	if (!unitSphereModel.load("Models\\UnitSphere\\UnitSphere.obj"))
		return false;


	// create g-buffer
	
	if (!Util::checkMemory(gBuffer = new GBuffer()))
		return false;

	if (!gBuffer->load(window->getWidth(), window->getHeight()))
		return false;


	// add a point light
	
	PointLight *l;

	if (!Util::checkMemory(l = new PointLight()))
		return false;

	l->position[0] = 0.f;
	l->position[1] = 200.f;
	l->position[2] = 0.f;
	l->diffuseColor[0] = 1.f;
	l->diffuseColor[1] = 1.f;
	l->diffuseColor[2] = .9f;
	l->diffuseIntensity = 10000.f;
	l->specularColor[0] = 1.f;
	l->specularColor[1] = 0.f;
	l->specularColor[2] = .0f;
	l->specularIntensity = 100000.f;
	l->specularPower = 500.f;
	l->attenuation[0] = 0.f;
	l->attenuation[1] = 0.f;
	l->attenuation[2] = 0.5f;
	
	pointLights.push_back(l);

	
	// and some spot lights

	for (int i = 0; i < 13; ++i)
	{
		SpotLight *s;
		
		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position[0] = 1075.f - i * 189.f;
		s->position[1] = 0.f;
		s->position[2] = 560.f;
		s->diffuseColor[0] = 1.f;
		s->diffuseColor[1] = 1.f;
		s->diffuseColor[2] = .9f;
		s->diffuseIntensity = 100000.f;
		s->specularColor[0] = 1.f;
		s->specularColor[1] = 0.f;
		s->specularColor[2] = .0f;
		s->specularIntensity = 100000.f;
		s->specularPower = 10.f;
		s->attenuation[0] = 0.f;
		s->attenuation[1] = 0.f;
		s->attenuation[2] = 1.f;
		s->direction[0] = 0.f;
		s->direction[1] = 1.f;
		s->direction[2] = 0.f;
		s->cutoffCosine = 25.f;
		spotLights.push_back(s);

		if (!Util::checkMemory(s = new SpotLight()))
			return false;

		s->position[0] = 1075.f - i * 189.f;
		s->position[1] = 0.f;
		s->position[2] = -630.f;
		s->diffuseColor[0] = 1.f;
		s->diffuseColor[1] = 1.f;
		s->diffuseColor[2] = .9f;
		s->diffuseIntensity = 100000.f;
		s->specularColor[0] = 1.f;
		s->specularColor[1] = 0.f;
		s->specularColor[2] = .0f;
		s->specularIntensity = 100000.f;
		s->specularPower = 10.f;
		s->attenuation[0] = 0.f;
		s->attenuation[1] = 0.f;
		s->attenuation[2] = 1.f;
		s->direction[0] = 0.f;
		s->direction[1] = 1.f;
		s->direction[2] = 0.f;
		s->cutoffCosine = 25.f;
		spotLights.push_back(s);
	}

	if (!Util::checkMemory(flashLight = new SpotLight()))
		return false;

	flashLight->diffuseColor[0] = .9f;
	flashLight->diffuseColor[1] = .9f;
	flashLight->diffuseColor[2] = 1.f;
	flashLight->diffuseIntensity = 30000.f;
	flashLight->specularColor[0] = 1.f;
	flashLight->specularColor[1] = 0.f;
	flashLight->specularColor[2] = .0f;
	flashLight->specularIntensity = 100000.f;
	flashLight->specularPower = 10.f;
	flashLight->attenuation[0] = 0.f;
	flashLight->attenuation[1] = 0.f;
	flashLight->attenuation[2] = 0.2f;
	flashLight->cutoffCosine = 30.f;
	
	return true;
}

float calcPointLightBSphere(const PointLight &l)
{
	float maxDiffuseChannel = fmax(fmax(l.diffuseColor[0], l.diffuseColor[1]), l.diffuseColor[2]);
	float maxSpecularChannel = fmax(fmax(l.specularColor[0], l.specularColor[1]), l.specularColor[2]);
	float maxChannel = fmax(maxDiffuseChannel, maxSpecularChannel);
	float maxIntensity = fmax(l.diffuseIntensity, l.specularIntensity);
	float s = (-l.attenuation[1] + sqrtf(l.attenuation[1] * l.attenuation[1] - 4 * l.attenuation[2] * (l.attenuation[0] - 256.f * maxChannel * maxIntensity))) / 2 * l.attenuation[2];
	return s;
}

void renderGeometryPass(float delta)
{
	glUseProgram(geometryShader->getProgram());

	gBuffer->bindForWriting();

	// make sure we write to the depth buffer
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	geometryShader->setWorldViewProjectionUniforms(glm::mat4(1.f), camera.getViewMatrix(), camera.getProjectionMatrix());

	sponzaModel.render(true);


	static float angle;
	glm::mat4 worldMatrix(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(-500.f, 0.f, 0.f));
	worldMatrix = glm::rotate(worldMatrix, angle, glm::vec3(0.f, 1.f, 0.f));
	angle += delta * .003f;

	geometryShader->setWorldViewProjectionUniforms(worldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());

	manModel.render(true);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

void renderGBufferDebug()
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

/*
void doShadowPass(float delta)
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

void beginLightPass()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	// light pass does not write to the depth buffer
	//glDepthMask(GL_FALSE);
	//glDisable(GL_DEPTH_TEST);

	assert(gBuffer);
	gBuffer->bindForReading(false);

	glClear(GL_COLOR_BUFFER_BIT);
}

void doDirectionalLightPass()
{
	glUseProgram(directionalLightingShader->getProgram());
	directionalLightingShader->setWorldViewProjectionUniforms(glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f));
	directionalLightingShader->setScreenSizeUniforms(window->getWidth(), window->getHeight());
	fullscreenQuadModel.render(false);
}

void doPointLightPass(float delta)
{
	glUseProgram(pointLightingShader->getProgram());

	for (PointLight *pointLight : pointLights)
	{
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(pointLight->position[0], pointLight->position[1], pointLight->position[2]));
		float s = calcPointLightBSphere(*pointLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		pointLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());
		pointLightingShader->setScreenSizeUniforms(window->getWidth(), window->getHeight());
		pointLightingShader->setLightPositionUniforms(pointLight->position[0], pointLight->position[1], pointLight->position[2]);
		pointLightingShader->setLightDiffuseUniforms(pointLight->diffuseColor[0], pointLight->diffuseColor[1], pointLight->diffuseColor[2], pointLight->diffuseIntensity);
		pointLightingShader->setLightSpecularUniforms(pointLight->specularColor[0], pointLight->specularColor[1], pointLight->specularColor[2], pointLight->specularIntensity, pointLight->specularPower);
		pointLightingShader->setLightAttenuationUniforms(pointLight->attenuation[0], pointLight->attenuation[1], pointLight->attenuation[2]);
		pointLightingShader->setEyePositionUniforms(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		
		unitSphereModel.render(false);
	}
}

void doSpotLightPass(float delta)
{
	glUseProgram(spotLightingShader->getProgram());

	for (SpotLight *spotLight : spotLights)
	{
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(spotLight->position[0], spotLight->position[1], spotLight->position[2]));
		float s = calcPointLightBSphere(*spotLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));

		spotLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());
		spotLightingShader->setScreenSizeUniforms(window->getWidth(), window->getHeight());
		spotLightingShader->setLightPositionUniforms(spotLight->position[0], spotLight->position[1], spotLight->position[2]);
		spotLightingShader->setLightDiffuseUniforms(spotLight->diffuseColor[0], spotLight->diffuseColor[1], spotLight->diffuseColor[2], spotLight->diffuseIntensity);
		spotLightingShader->setLightSpecularUniforms(spotLight->specularColor[0], spotLight->specularColor[1], spotLight->specularColor[2], spotLight->specularIntensity, spotLight->specularPower);
		spotLightingShader->setLightDirectionUniforms(spotLight->direction[0], spotLight->direction[1], spotLight->direction[2]);
		spotLightingShader->setLightCutoffCosineUniform(spotLight->cutoffCosine);
		spotLightingShader->setLightAttenuationUniforms(spotLight->attenuation[0], spotLight->attenuation[1], spotLight->attenuation[2]);
		spotLightingShader->setEyePositionUniforms(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		unitSphereModel.render(false);
	}

	if (input.isFlashLightOn())
	{
		flashLight->position[0] = camera.getPosition().x;
		flashLight->position[1] = camera.getPosition().y;
		flashLight->position[2] = camera.getPosition().z;

		flashLight->direction[0] = camera.getForward().x;
		flashLight->direction[1] = camera.getForward().y;
		flashLight->direction[2] = camera.getForward().z;

		// set the world matrix
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(flashLight->position[0], flashLight->position[1], flashLight->position[2]));
		float s = calcPointLightBSphere(*flashLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		
		spotLightingShader->setWorldViewProjectionUniforms(worldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());
		spotLightingShader->setScreenSizeUniforms(window->getWidth(), window->getHeight());
		spotLightingShader->setLightPositionUniforms(flashLight->position[0], flashLight->position[1], flashLight->position[2]);
		spotLightingShader->setLightDiffuseUniforms(flashLight->diffuseColor[0], flashLight->diffuseColor[1], flashLight->diffuseColor[2], flashLight->diffuseIntensity);
		spotLightingShader->setLightSpecularUniforms(flashLight->specularColor[0], flashLight->specularColor[1], flashLight->specularColor[2], flashLight->specularIntensity, flashLight->specularPower);
		spotLightingShader->setLightDirectionUniforms(flashLight->direction[0], flashLight->direction[1], flashLight->direction[2]);
		spotLightingShader->setLightCutoffCosineUniform(flashLight->cutoffCosine);
		spotLightingShader->setLightAttenuationUniforms(flashLight->attenuation[0], flashLight->attenuation[1], flashLight->attenuation[2]);
		spotLightingShader->setEyePositionUniforms(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		unitSphereModel.render(false);
	}
}

void render(float delta)
{
	camera.update(input, delta, window->getWidth(), window->getHeight());

	static float z = 0.f;
	pointLights[0]->position[0] = sinf(z) * 1200.f;
	z += .005f * delta;

	renderGeometryPass(delta);

	if (input.isDebugModeOn())
		renderGBufferDebug();
	else
	{
		//doShadowPass(delta);

		beginLightPass();
		doDirectionalLightPass();
		doPointLightPass(delta);
		doSpotLightPass(delta);
	}

	renderer->finalizeFrame();
}

void destroy()
{
	delete gBuffer;

	delete geometryShader;
	delete directionalLightingShader;
	delete pointLightingShader;
	delete spotLightingShader;

	delete renderer;

	delete window;
}

int main(int argc, char **argv)
{
	if (!load())
		return -1;

	while (window->getAlive())
	{
		unsigned int thisTickTime = SDL_GetTicks();
		float delta = (thisTickTime - lastTickTime) * .1f;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			// quit event
				window->setAlive(false);
			else if (event.type == SDL_MOUSEMOTION)
			// mouse input
			{
				int w2 = window->getWidth() / 2;
				int h2 = window->getHeight() / 2;

				camera.rotateYaw((event.motion.x - w2) / (float)w2);
				camera.rotatePitch((event.motion.y - h2) / (float)h2);

				SDL_WarpMouseInWindow(window->getSDLWindow(), w2, h2);
			}
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			// keyboard input
			{
				if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP)
					window->setAlive(false);
				else if (event.key.keysym.sym == SDLK_F10 && event.type == SDL_KEYUP)
					window->changeResolution(window->getWidth(), window->getHeight(), !window->getFullscreen());
				else
					input.sendKeyboardEvent(event);
			}
		}

		render(delta);

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}