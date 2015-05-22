#include <glew.h>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

#include "BillboardRenderer.hpp"
#include "..\Light\PointLight.hpp"
#include "..\Light\SpotLight.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

extern std::vector<PointLight*> pointLights;
extern std::vector<SpotLight*> spotLights;

bool BillboardRenderer::create()
{
	if (!Util::checkMemory(billboardDrawShader = new BillboardDrawShader()))
		return false;

	if (!billboardDrawShader->create())
		return false;

	if (!Util::checkMemory(texture = new Texture()))
		return false;

	if (!texture->load("Textures\\BillboardPointlight.png"))
		return false;
	
	float data[4][4] =
	{
		{ -1.f,  1.f, 0.f, 0.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f,   1.f, 1.f, 0.f },
		{ 1.f,  -1.f, 1.f, 1.f }
	};

	// generate and bind a VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// generate and bind a VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	// set vertex attribute array pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);								// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));	// tex coord uvs

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate VBO and VAO for billboard renderer: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

BillboardRenderer::~BillboardRenderer()
{
	delete texture;
	delete billboardDrawShader;
}

void BillboardRenderer::render(Camera *camera)
{
	glUseProgram(billboardDrawShader->getProgram());

	for (PointLight *l : pointLights)
	{
		glm::mat4 bmatrix;

		glm::vec3 look;

		look.x = camera->getPosition().x - l->position[0];
		look.y = camera->getPosition().y - l->position[1];
		look.z = camera->getPosition().z - l->position[2];

		glm::normalize(look);

		bmatrix[0] = glm::vec4(camera->getRight(), 0.f);
		bmatrix[1] = glm::vec4(camera->getUp(), 0.f);
		bmatrix[2] = glm::vec4(look, 0.f);

		bmatrix[3].x = l->position[0];
		bmatrix[3].y = l->position[1];
		bmatrix[3].z = l->position[2];
		bmatrix[3].w = 1.f;
		
		bmatrix = glm::scale(bmatrix, glm::vec3(35.f, 35.f, 35.f));

		billboardDrawShader->setWorldViewProjectionMatrixUniforms(bmatrix, camera->getViewMatrix(), camera->getProjectionMatrix());
		billboardDrawShader->setTintColorUniform(l->diffuseColor[0], l->diffuseColor[1], l->diffuseColor[2]);
		
		glBindVertexArray(vao);

		texture->bind(GL_TEXTURE0);
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}