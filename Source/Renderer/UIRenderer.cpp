#include <glew.h>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <SDL_opengl.h>

#include "UIRenderer.hpp"
#include "..\Renderer\UnitQuad.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"
#include "..\Window\Window.hpp"

extern Window *window;

const std::string UIRenderer::FONT_FILENAME = "Fonts\\OpenSans\\OpenSans-Semibold.ttf";

bool UIRenderer::loadFont()
{
	if (TTF_Init() < 0)
	{
		Error::report("Error", "SDL_ttf could not be initialized: " + std::string(TTF_GetError()));
		return false;
	}

	font = TTF_OpenFont(FONT_FILENAME.c_str(), 20);

	if (!font)
	{
		Error::report("Error", "Failed to load font: \"" + FONT_FILENAME + "\" (" + std::string(TTF_GetError()) + ")");
		return false;
	}

	glGenTextures(1, &fontTextureID);

	return true;
}

bool UIRenderer::create()
{
	if (!loadFont())
		return false;

	if (!Util::checkMemory(uiDrawShader = new UIDrawShader()))
		return false;

	if (!uiDrawShader->create())
		return false;

	return true;
}

UIRenderer::~UIRenderer()
{
	delete uiDrawShader;

	if (font)
		TTF_CloseFont(font);

	TTF_Quit();
}

void UIRenderer::drawText(std::string text, glm::vec2 position, SDL_Color color)
{
	// convert RGB color to BGR
	int swap = color.r;
	color.r = color.b;
	color.b = swap;

	SDL_Surface *sText = TTF_RenderUTF8_Blended(font, text.c_str(), color);

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

	glUseProgram(uiDrawShader->program);

	glm::mat4 worldMatrix;
	worldMatrix = glm::translate(worldMatrix, glm::vec3(((position.x + sText->w / 2.f) / window->width) * 2.f - 1.f, -((position.y + sText->h / 2.f) / window->height) * 2.f + 1.f, 0.f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(float(sText->w) / window->width, sText->h / float(window->height), 1.f));
	uiDrawShader->setWorldMatrixUniform(worldMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sText->w, sText->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	UnitQuad::render();

	SDL_FreeSurface(sText);
	SDL_FreeSurface(temp);
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