#include "Camera.hpp"
#include "Error.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "Window.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_opengl.h>

Camera *camera;
Model *sponzaModel, *bannerModel;
Renderer *renderer;
ShaderProgram *simpleDrawShaderProgram;
Window *window;

unsigned int lastTickTime = 0;
GLint worldViewMatrixLocation;

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


	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to load tree draw shader: " + Error::getOpenGLErrorString(error));
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

	// load shader
	if (!initSimpleDrawShaderProgram())
		return false;


	// load models

	sponzaModel = new Model();

	if (!sponzaModel->load("Models\\sponza\\sponza.obj"))
		return false;

	glm::mat4 worldMatrix;
	worldMatrix = glm::scale(worldMatrix, glm::vec3(1.f, 1.f, 1.f));
	sponzaModel->setWorldMatrix(worldMatrix);

	bannerModel = new Model();

	if (!bannerModel->load("Models\\sponza\\banner.obj"))
		return false;

	worldMatrix = glm::rotate(worldMatrix, 90.f, glm::vec3(1.f, 0.f, 0.f));
	bannerModel->setWorldMatrix(worldMatrix);
	

	return true;
}

void render(float delta)
{
	renderer->beginDrawing();

	glUseProgram(simpleDrawShaderProgram->getProgram());
	
	glUniformMatrix4fv(worldViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix(delta) * sponzaModel->getWorldMatrix()));
	sponzaModel->render();
	
	glUniformMatrix4fv(worldViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix(delta) * bannerModel->getWorldMatrix()));
	bannerModel->render();
	
	renderer->endDrawing();
}

void destroy()
{
	delete sponzaModel;
	delete bannerModel;
	delete simpleDrawShaderProgram;
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