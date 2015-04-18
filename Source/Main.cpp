#include "Camera.hpp"
#include "Error.hpp"
#include "GBuffer.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_opengl.h>

Camera *camera;
GBuffer *gBuffer;
Model *fullscreenQuadModel, *sponzaModel;
Renderer *renderer;
ShaderProgram /**simpleDrawShaderProgram,*/ *geometryPassShaderProgram, *directionalLightingPassShaderProgram;
Window *window;

unsigned int lastTickTime = 0;

//GLint worldViewMatrixLocation;
GLint gp_worldMatrixLocation, gp_viewProjectionMatrixLocation;
GLint dlp_worldViewProjectionMatrixLocation;

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

	if (!directionalLightingPassShaderProgram->load("Shaders\\DirectionalLightingPass.vs.glsl", "", "Shaders\\DirectionalLightingPass.fs.glsl"))
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

bool load()
{
	// create a window

	window = new Window();

	assert(window);

	if (!window->create(1600, 900))
		return false;


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


	// create camera
	camera = new Camera();


	// load shaders
	
	//if (!initSimpleDrawShaderProgram())
		//return false;

	if (!initGeometryPassShaderProgram())
		return false;

	if (!initDirectionalLightingPassShaderProgram())
		return false;


	// load models

	sponzaModel = new Model();

	if (!sponzaModel->load("Models\\sponza2\\sponza.obj"))
		return false;

	glm::mat4 worldMatrix;
	worldMatrix = glm::scale(worldMatrix, glm::vec3(1.f, 1.f, 1.f));
	sponzaModel->setWorldMatrix(worldMatrix);


	fullscreenQuadModel = new Model();

	if (!fullscreenQuadModel->load("Models\\fullscreen_quad\\fullscreen_quad.obj"))
		return false;

	worldMatrix = glm::scale(worldMatrix, glm::vec3(1.f, 1.f, 1.f));
	fullscreenQuadModel->setWorldMatrix(worldMatrix);


	// create g-buffer
	
	gBuffer = new GBuffer();

	if (!gBuffer->load(window->getWidth(), window->getHeight()))
		return false;
	

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

	// set the world matrix
	glUniformMatrix4fv(gp_worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(sponzaModel->getWorldMatrix()));

	// set the view-projection matrix
	glm::mat4 projection_matrix(glm::perspective(glm::radians(74.f), (float)window->getWidth() / (float)window->getHeight(), 1.0f, 10000.0f));
	glUniformMatrix4fv(gp_viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix * camera->getViewMatrix(delta)));

	sponzaModel->render(true);
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
	// light pass does not write to the depth buffer
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	assert(gBuffer);
	gBuffer->bindForReading(false);

	glClear(GL_COLOR_BUFFER_BIT);
}

void doDirectionalLightPass()
{
	glUseProgram(directionalLightingPassShaderProgram->getProgram());

	// set the world matrix
	glUniformMatrix4fv(dlp_worldViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	fullscreenQuadModel->render(false);
}

void render(float delta)
{
	renderGeometryPass(delta);

	if (camera->isDebugModeOn())
		renderGBufferDebug();
	else
	{
		beginLightPass();
		doDirectionalLightPass();
	}

	renderer->finalizeFrame();
}

void destroy()
{
	delete gBuffer;
	delete fullscreenQuadModel;
	delete sponzaModel;
	//delete simpleDrawShaderProgram;
	delete camera;
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

				camera->rotateYaw((e.motion.x - w2) / (float)w2);
				camera->rotatePitch((e.motion.y - h2) / (float)h2);

				SDL_WarpMouseInWindow(window->getSDLWindow(), w2, h2);
			}
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			// keyboard input
			{
				if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYUP)
					window->setAlive(false);
				else if (e.key.keysym.sym == SDLK_TAB && e.type == SDL_KEYDOWN)
					camera->toggleDebugMode();
				else if (e.key.keysym.sym == SDLK_F10 && e.type == SDL_KEYUP)
					window->changeResolution(window->getWidth(), window->getHeight(), !window->getFullscreen());
				else if (e.key.keysym.sym == SDLK_w)
					camera->setForwardKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_s)
					camera->setBackKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_a)
					camera->setLeftKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_d)
					camera->setRightKeyPressed(e.type == SDL_KEYDOWN);
				else if (e.key.keysym.sym == SDLK_LSHIFT)
					camera->setCrouchKeyPressed(e.type == SDL_KEYDOWN);
			}
		}

		render(delta);

		lastTickTime = thisTickTime;
	}

	destroy();

	return 0;
}