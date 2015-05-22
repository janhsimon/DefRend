#pragma once

#include <SDL_ttf.h>
#include <string>

#include "..\Shader\BillboardDrawShader.hpp"
#include "..\Camera\Camera.hpp"
#include "..\Material\Texture.hpp"

class BillboardRenderer
{
private:
	BillboardDrawShader *billboardDrawShader;
	Texture *texture;
	GLuint vbo;
	GLuint vao;

public:
	bool create();
	~BillboardRenderer();

	void render(Camera *camera);
};