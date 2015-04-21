#include "Camera.hpp"
#include "Error.hpp"
#include "GBuffer.hpp"
#include "Model.hpp"
#include "PointLight.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "SpotLight.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_opengl.h>

Camera camera;
GBuffer *gBuffer;
Model fullscreenQuadModel, sponzaModel, manModel, unitSphereModel;
Renderer *renderer;
ShaderProgram /**simpleDrawShaderProgram,*/ *geometryPassShaderProgram, *directionalLightingPassShaderProgram, *pointLightingPassShaderProgram, *spotLightingPassShaderProgram;
Window *window;

std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;

unsigned int lastTickTime = 0;

glm::mat4 projection_matrix;

//GLint worldViewMatrixLocation;
GLint gp_worldMatrixLocation, gp_viewProjectionMatrixLocation;
GLint dlp_worldViewProjectionMatrixLocation;
GLint plp_worldViewProjectionMatrixLocation, plp_lightPosition, plp_lightColor, plp_lightIntensity, plp_lightAttenuation;
GLint slp_worldViewProjectionMatrixLocation, slp_lightPosition, slp_lightColor, slp_lightIntensity, slp_lightAttenuation, slp_lightDirection, slp_lightCutoffCosine;

/*
bool initSimpleDrawShaderProgram()
{
	simpleDrawShaderProgram = new ShaderProgram();

	assert(simpleDrawShaderProgram);

	if (!simpleDrawShaderProgram->load("Shaders\\SimpleDraw.vs.glsl", "", "Shaders\\SimpleDraw.fs.glsl"))
		return false;

	if (!simpleDrawShaderProgram->link())
		return false;

	glUseProgram(simpleDrawShaderProgram->getProgram());

	if (!simpleDrawShaderProgram->getUniformLocation("worldViewMatrix", worldViewMatrixLocation))
		return false;


	// set up the projection matrix

	GLint projectionMatrixLocation;
	if (!simpleDrawShaderProgram->getUniformLocation("projectionMatrix", projectionMatrixLocation))
		return false;
	
	glm::mat4 projection_matrix(glm::perspective(glm::radians(74.f), (float)window->getWidth() / (float)window->getHeight(), 1.0f, 10000.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix));


	// set up texture units

	GLint texLoc;
	if (!simpleDrawShaderProgram->getUniformLocation("diffuseMap", texLoc))
		return false;

	glUniform1i(texLoc, 0);

	if (!simpleDrawShaderProgram->getUniformLocation("normalMap", texLoc))
		return false;

	glUniform1i(texLoc, 1);

	if (!simpleDrawShaderProgram->getUniformLocation("opacityMap", texLoc))
		return false;

	glUniform1i(texLoc, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load tree draw shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}
*/

bool initGeometryPassShaderProgram()
{
	geometryPassShaderProgram = new ShaderProgram();

	assert(geometryPassShaderProgram);

	if (!geometryPassShaderProgram->load("Shaders\\GeometryPass.vs.glsl", "", "Shaders\\GeometryPass.fs.glsl"))
		return false;

	if (!geometryPassShaderProgram->link())
		return false;

	glUseProgram(geometryPassShaderProgram->getProgram());

	if (!geometryPassShaderProgram->getUniformLocation("worldMatrix", gp_worldMatrixLocation))
		return false;

	if (!geometryPassShaderProgram->getUniformLocation("viewProjectionMatrix", gp_viewProjectionMatrixLocation))
		return false;


	// set up texture units

	GLint texLoc;
	if (!geometryPassShaderProgram->getUniformLocation("diffuseMap", texLoc))
		return false;

	glUniform1i(texLoc, 0);

	if (!geometryPassShaderProgram->getUniformLocation("normalMap", texLoc))
		return false;

	glUniform1i(texLoc, 1);

	if (!geometryPassShaderProgram->getUniformLocation("opacityMap", texLoc))
		return false;

	glUniform1i(texLoc, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load deferred geometry pass shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

bool initDirectionalLightingPassShaderProgram()
{
	directionalLightingPassShaderProgram = new ShaderProgram();

	assert(directionalLightingPassShaderProgram);

	if (!directionalLightingPassShaderProgram->load("Shaders\\LightingPass.vs.glsl", "", "Shaders\\DirectionalLightingPass.fs.glsl"))
		return false;

	if (!directionalLightingPassShaderProgram->link())
		return false;

	glUseProgram(directionalLightingPassShaderProgram->getProgram());

	if (!directionalLightingPassShaderProgram->getUniformLocation("worldViewProjectionMatrix", dlp_worldViewProjectionMatrixLocation))
		return false;


	// set up screen size

	GLint screenSizeLocation;
	if (!directionalLightingPassShaderProgram->getUniformLocation("screenSize", screenSizeLocation))
		return false;

	glUniform2f(screenSizeLocation, (float)window->getWidth(), (float)window->getHeight());


	// set up texture units

	GLint texLoc;
	if (!directionalLightingPassShaderProgram->getUniformLocation("gbuffer_worldPosition", texLoc))
		return false;

	glUniform1i(texLoc, 0);

	if (!directionalLightingPassShaderProgram->getUniformLocation("gbuffer_diffuse", texLoc))
		return false;

	glUniform1i(texLoc, 1);

	if (!directionalLightingPassShaderProgram->getUniformLocation("gbuffer_normal", texLoc))
		return false;

	glUniform1i(texLoc, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load deferred direction lighting pass shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

bool initPointLightingPassShaderProgram()
{
	pointLightingPassShaderProgram = new ShaderProgram();

	assert(pointLightingPassShaderProgram);

	if (!pointLightingPassShaderProgram->load("Shaders\\LightingPass.vs.glsl", "", "Shaders\\PointLightingPass.fs.glsl"))
		return false;

	if (!pointLightingPassShaderProgram->link())
		return false;

	
	glUseProgram(pointLightingPassShaderProgram->getProgram());

	if (!pointLightingPassShaderProgram->getUniformLocation("worldViewProjectionMatrix", plp_worldViewProjectionMatrixLocation))
		return false;


	// set up screen size

	GLint screenSizeLocation;
	if (!pointLightingPassShaderProgram->getUniformLocation("screenSize", screenSizeLocation))
		return false;

	glUniform2f(screenSizeLocation, (float)window->getWidth(), (float)window->getHeight());


	// retrieve point light parameters

	if (!pointLightingPassShaderProgram->getUniformLocation("lightPosition", plp_lightPosition))
		return false;

	if (!pointLightingPassShaderProgram->getUniformLocation("lightColor", plp_lightColor))
		return false;

	if (!pointLightingPassShaderProgram->getUniformLocation("lightIntensity", plp_lightIntensity))
		return false;

	if (!pointLightingPassShaderProgram->getUniformLocation("lightAttenuation", plp_lightAttenuation))
		return false;


	// set up texture units

	GLint texLoc;
	if (!pointLightingPassShaderProgram->getUniformLocation("gbuffer_worldPosition", texLoc))
		return false;

	glUniform1i(texLoc, 0);

	if (!pointLightingPassShaderProgram->getUniformLocation("gbuffer_diffuse", texLoc))
		return false;

	glUniform1i(texLoc, 1);

	if (!pointLightingPassShaderProgram->getUniformLocation("gbuffer_normal", texLoc))
		return false;

	glUniform1i(texLoc, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load deferred point lighting pass shader: " + Error::getOpenGLErrorString(error));
		return false;
	}
	
	return true;
}

bool initSpotLightingPassShaderProgram()
{
	spotLightingPassShaderProgram = new ShaderProgram();

	assert(spotLightingPassShaderProgram);

	if (!spotLightingPassShaderProgram->load("Shaders\\LightingPass.vs.glsl", "", "Shaders\\SpotLightingPass.fs.glsl"))
		return false;

	if (!spotLightingPassShaderProgram->link())
		return false;


	glUseProgram(spotLightingPassShaderProgram->getProgram());

	if (!spotLightingPassShaderProgram->getUniformLocation("worldViewProjectionMatrix", slp_worldViewProjectionMatrixLocation))
		return false;


	// set up screen size

	GLint screenSizeLocation;
	if (!spotLightingPassShaderProgram->getUniformLocation("screenSize", screenSizeLocation))
		return false;

	glUniform2f(screenSizeLocation, (float)window->getWidth(), (float)window->getHeight());


	// retrieve point light parameters

	if (!spotLightingPassShaderProgram->getUniformLocation("lightPosition", slp_lightPosition))
		return false;

	if (!spotLightingPassShaderProgram->getUniformLocation("lightColor", slp_lightColor))
		return false;

	if (!spotLightingPassShaderProgram->getUniformLocation("lightIntensity", slp_lightIntensity))
		return false;

	if (!spotLightingPassShaderProgram->getUniformLocation("lightAttenuation", slp_lightAttenuation))
		return false;

	if (!spotLightingPassShaderProgram->getUniformLocation("lightDirection", slp_lightDirection))
		return false;

	if (!spotLightingPassShaderProgram->getUniformLocation("lightCutoffCosine", slp_lightCutoffCosine))
		return false;


	// set up texture units

	GLint texLoc;
	if (!spotLightingPassShaderProgram->getUniformLocation("gbuffer_worldPosition", texLoc))
		return false;

	glUniform1i(texLoc, 0);

	if (!spotLightingPassShaderProgram->getUniformLocation("gbuffer_diffuse", texLoc))
		return false;

	glUniform1i(texLoc, 1);

	if (!spotLightingPassShaderProgram->getUniformLocation("gbuffer_normal", texLoc))
		return false;

	glUniform1i(texLoc, 2);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load deferred spot lighting pass shader: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

bool load()
{
	// create a window

	window = new Window();

	assert(window);

	if (!window->create(1600, 900))
		return false;

	// set up the projection matrix
	projection_matrix = glm::perspective(glm::radians(74.f), (float)window->getWidth() / (float)window->getHeight(), 1.0f, 10000.0f);


	// create a renderer

	renderer = new Renderer();

	assert(renderer);

	if (!renderer->create())
		return false;

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
	
	//if (!initSimpleDrawShaderProgram())
		//return false;

	if (!initGeometryPassShaderProgram())
		return false;

	if (!initDirectionalLightingPassShaderProgram())
		return false;

	if (!initPointLightingPassShaderProgram())
		return false;

	if (!initSpotLightingPassShaderProgram())
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
	
	gBuffer = new GBuffer();

	if (!gBuffer->load(window->getWidth(), window->getHeight()))
		return false;


	// add a point light
	
	PointLight *l = new PointLight();
	l->position[0] = 0.f;
	l->position[1] = 0.f;
	l->position[2] = 0.f;
	l->color[0] = 1.f;
	l->color[1] = 0.f;
	l->color[2] = 0.f;
	l->intensity = 100000.f;
	l->attenuation[0] = 0.f;
	l->attenuation[1] = 0.f;
	l->attenuation[2] = 1.f;
	pointLights.push_back(l);

	for (int i = 0; i < 13; ++i)
	{
		SpotLight *s = new SpotLight();
		s->position[0] = 1075.f - i * 189.f;
		s->position[1] = 0.f;
		s->position[2] = 560.f;
		s->color[0] = .9f;
		s->color[1] = .9f;
		s->color[2] = .6f;
		s->intensity = 100000.f;
		s->attenuation[0] = 0.f;
		s->attenuation[1] = 0.f;
		s->attenuation[2] = 1.f;
		s->direction[0] = 0.f;
		s->direction[1] = 1.f;
		s->direction[2] = 0.f;
		s->cutoff = 25.f;
		spotLights.push_back(s);

		s = new SpotLight();
		s->position[0] = 1075.f - i * 189.f;
		s->position[1] = 0.f;
		s->position[2] = -630.f;
		s->color[0] = .9f;
		s->color[1] = .9f;
		s->color[2] = .6f;
		s->intensity = 100000.f;
		s->attenuation[0] = 0.f;
		s->attenuation[1] = 0.f;
		s->attenuation[2] = 1.f;
		s->direction[0] = 0.f;
		s->direction[1] = 1.f;
		s->direction[2] = 0.f;
		s->cutoff = 25.f;
		spotLights.push_back(s);
	}

	return true;
}

void renderGeometryPass(float delta)
{
	glUseProgram(geometryPassShaderProgram->getProgram());

	gBuffer->bindForWriting();

	// make sure we write to the depth buffer
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// set the view-projection matrix
	glUniformMatrix4fv(gp_viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix * camera.getViewMatrix()));


	// set the world matrix
	glUniformMatrix4fv(gp_worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	sponzaModel.render(true);


	static float angle;
	glm::mat4 worldMatrix(1.f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(-500.f, 0.f, 0.f));
	worldMatrix = glm::rotate(worldMatrix, angle, glm::vec3(0.f, 1.f, 0.f));
	angle += delta * .003f;

	// set the world matrix
	glUniformMatrix4fv(gp_worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));

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

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), 0, halfHeight, halfWidth, window->getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->setReadBuffer(GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), halfWidth, halfHeight, window->getWidth(), window->getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

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
	glUseProgram(directionalLightingPassShaderProgram->getProgram());

	// set the world matrix
	glUniformMatrix4fv(dlp_worldViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	fullscreenQuadModel.render(false);
}

float calcPointLightBSphere(const PointLight& l)
{
	float maxChannel = fmax(fmax(l.color[0], l.color[1]), l.color[2]);
	return (-l.attenuation[1] + sqrtf(l.attenuation[1] * l.attenuation[1] - 4 * l.attenuation[2] * (/*TODO: check should this be constant?*/l.attenuation[0] - 256.f * maxChannel * l.intensity))) / 2 * l.attenuation[2];
}

void doPointLightPass(float delta)
{
	glUseProgram(pointLightingPassShaderProgram->getProgram());

	for (PointLight *pointLight : pointLights)
	{
		// set the world matrix
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(pointLight->position[0], pointLight->position[1], pointLight->position[2]));
		float s = calcPointLightBSphere(*pointLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		glUniformMatrix4fv(plp_worldViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix * camera.getViewMatrix() * worldMatrix));

		glUniform3f(plp_lightPosition, pointLight->position[0], pointLight->position[1], pointLight->position[2]);
		glUniform3f(plp_lightColor, pointLight->color[0], pointLight->color[1], pointLight->color[2]);
		glUniform1f(plp_lightIntensity, pointLight->intensity);
		glUniform3f(plp_lightAttenuation, pointLight->attenuation[0], pointLight->attenuation[1], pointLight->attenuation[2]);

		unitSphereModel.render(false);
	}
}

void doSpotLightPass(float delta)
{
	glUseProgram(spotLightingPassShaderProgram->getProgram());

	for (SpotLight *spotLight : spotLights)
	{
		// set the world matrix
		glm::mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, glm::vec3(spotLight->position[0], spotLight->position[1], spotLight->position[2]));
		float s = calcPointLightBSphere(*spotLight);
		worldMatrix = glm::scale(worldMatrix, glm::vec3(s, s, s));
		glUniformMatrix4fv(slp_worldViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix * camera.getViewMatrix() * worldMatrix));

		glUniform3f(slp_lightPosition, spotLight->position[0], spotLight->position[1], spotLight->position[2]);
		glUniform3f(slp_lightColor, spotLight->color[0], spotLight->color[1], spotLight->color[2]);
		glUniform1f(slp_lightIntensity, spotLight->intensity);
		glUniform3f(slp_lightAttenuation, spotLight->attenuation[0], spotLight->attenuation[1], spotLight->attenuation[2]);
		glUniform3f(slp_lightDirection, spotLight->direction[0], spotLight->direction[1], spotLight->direction[2]);
		glUniform1f(slp_lightCutoffCosine, cosf(glm::radians(spotLight->cutoff)));

		unitSphereModel.render(false);
	}
}

void render(float delta)
{
	camera.update(delta);

	renderGeometryPass(delta);

	if (camera.isDebugModeOn())
		renderGBufferDebug();
	else
	{
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
	//delete simpleDrawShaderProgram;
	delete geometryPassShaderProgram;
	delete directionalLightingPassShaderProgram;
	delete pointLightingPassShaderProgram;
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

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			// quit event
				window->setAlive(false);
			else if (e.type == SDL_MOUSEMOTION)
			// mouse input
			{
				int w2 = window->getWidth() / 2;
				int h2 = window->getHeight() / 2;

				camera.rotateYaw((e.motion.x - w2) / (float)w2);
				camera.rotatePitch((e.motion.y - h2) / (float)h2);

				SDL_WarpMouseInWindow(window->getSDLWindow(), w2, h2);
			}
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			// keyboard input
			{
				if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYUP)
					window->setAlive(false);
				else if (e.key.keysym.sym == SDLK_TAB && e.type == SDL_KEYDOWN)
					camera.toggleDebugMode();
				else if (e.key.keysym.sym == SDLK_F10 && e.type == SDL_KEYUP)
					window->changeResolution(window->getWidth(), window->getHeight(), !window->getFullscreen());
				else if (e.key.keysym.sym == SDLK_w)
					camera.setForwardKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_s)
					camera.setBackKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_a)
					camera.setLeftKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_d)
					camera.setRightKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_LSHIFT)
					camera.setCrouchKeyPressed(e.type == SDL_KEYDOWN);

				else if (e.key.keysym.sym == SDLK_q && e.type == SDL_KEYUP)
					pointLights[0]->intensity--;
				else if (e.key.keysym.sym == SDLK_e && e.type == SDL_KEYUP)
					pointLights[0]->intensity *= pointLights[0]->intensity;

				else if (e.key.keysym.sym == SDLK_z && e.type == SDL_KEYUP)
					pointLights[0]->attenuation[2]--;
				else if (e.key.keysym.sym == SDLK_c && e.type == SDL_KEYUP)
					pointLights[0]->attenuation[2]++;
			}
		}

		render(delta);

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}