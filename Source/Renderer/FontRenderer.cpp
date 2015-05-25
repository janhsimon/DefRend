#include <glew.h>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <SDL_opengl.h>

#include "FontRenderer.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

bool FontRenderer::create()
{
	std::string fontFilename = "Fonts\\Munro.ttf";

	if (TTF_Init() < 0)
	{
		Error::report("Error", "SDL_ttf could not be initialized: " + std::string(TTF_GetError()));
		return false;
	}

	font = TTF_OpenFont(fontFilename.c_str(), 20);

	if (!font)
	{
		Error::report("Error", "Failed to load font: \"" + fontFilename + "\" (" + std::string(TTF_GetError()) + ")");
		return false;
	}

	glGenTextures(1, &textureID);

	if (!Util::checkMemory(fontDrawShader = new FontDrawShader()))
		return false;

	if (!fontDrawShader->create())
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
		Error::report("Error", "Failed to generate VBO and VAO for font renderer: " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

FontRenderer::~FontRenderer()
{
	delete fontDrawShader;

	if (font)
	{
		TTF_CloseFont(font);
		font = nullptr;
	}

	TTF_Quit();
}

void FontRenderer::drawFontText(TTF_Font *f, std::string text, float x, float y, SDL_Color color)
{
	// convert RGB color to BGR
	int swap = color.r;
	color.r = color.b;
	color.b = swap;

	SDL_Surface *sText = TTF_RenderUTF8_Blended(f, text.c_str(), color);

	if (!sText)
	{
		//g_pLogfile->fLog("Unable to render text surface: %s", SDL_GetError());
		return;
	}

	SDL_Rect area = { 0, 0, sText->w, sText->h };

	SDL_Surface *temp = SDL_CreateRGBSurface(0, sText->w, sText->h, 32, 0, 0, 0, 0);

	if (!temp)
	{
		//g_pLogfile->fLog("Unable to create text surface: %s", SDL_GetError());
		return;
	}

	SDL_BlitSurface(sText, &area, temp, NULL);

	glUseProgram(fontDrawShader->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((x + sText->w / 2.f) / 1600.f) * 2.f - 1.f, ((y + sText->h / 2.f) / 900.f) * 2.f - 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(sText->w / 1600.f, sText->h / 900.f, 1.f));
	fontDrawShader->setWorldMatrixUniform(worldMatrix);

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sText->w, sText->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	SDL_FreeSurface(sText);
	SDL_FreeSurface(temp);
}

void FontRenderer::drawText(std::string text, float x, float y, SDL_Color color)
{
	drawFontText(font, text, x, y, color);
}

/*
Vector2 FontRenderer::getTextDimensions(std::string text)
{
	SDL_Surface *sText = TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color());
	Vector2 out = Vector2(sText->w, sText->h);
	SDL_FreeSurface(sText);
	return out;
}
*/