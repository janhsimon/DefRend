#pragma once

#include <assimp/material.h>

#include "Texture.hpp"
#include "TextureManager.hpp"

class Material
{
private:
	std::string name;
	Texture *diffuseMap, *specularMap, *normalMap, *opacityMap;

	void parseName(aiMaterial *material);
	bool parseTexture(aiMaterial *material, const std::string &modelFilename, int slot);

public:
	~Material();

	bool load(aiMaterial *material, const std::string &modelFilename);
	void bind();
	std::string getName();
};