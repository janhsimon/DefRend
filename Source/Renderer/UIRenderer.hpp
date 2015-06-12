#pragma once

#include <SDL_ttf.h>
#include <string>

#include "..\Shader\UIDrawShader.hpp"

class UIRenderer
{
private:
	static const std::string FONT_FILENAME;
	TTF_Font *font;
	GLuint fontTextureID;
	UIDrawShader *uiDrawShader;

	bool loadFont();

public:
	bool create();
	~UIRenderer();

	void drawText(std::string text, glm::vec2 position, SDL_Color color);
	
	glm::vec2 getTextDimensions(const std::string &text);

	inline UIDrawShader *getUIDrawShader() const { return uiDrawShader; }
};