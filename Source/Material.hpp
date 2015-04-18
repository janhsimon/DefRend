#pragma once

#include "Texture.hpp"
#include "TextureManager.hpp"

#include <assimp/material.h>

class Material
{
private:
	std::string name;
	Texture *diffuseMap, *normalMap, *opacityMap;

	void parseName(aiMaterial *material);
	bool parseTexture(aiMaterial *material, int slot);

public:
	~Material();

	bool load(aiMaterial *material);
	void bind();
	std::string getName();
};