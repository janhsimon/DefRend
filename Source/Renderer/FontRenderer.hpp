#pragma once

#include <SDL_ttf.h>
#include <string>

#include "..\Shader\FontDrawShader.hpp"

class FontRenderer
{
private:
	TTF_Font *font;
	GLuint textureID;
	FontDrawShader *fontDrawShader;
	GLuint vbo;
	GLuint vao;

	void drawFontText(TTF_Font *f, std::string text, float x, float y, SDL_Color color);

public:
	bool create();
	~FontRenderer();

	void render(SDL_Renderer *renderer, std::string text, SDL_Color color);
	void drawText(std::string text, float x, float y, SDL_Color color);
	
	//Vector2 getTextDimensions(std::string text);
};