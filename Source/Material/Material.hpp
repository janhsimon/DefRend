#pragma once

#include <assimp\material.h>

#include "Texture.hpp"
#include "TextureManager.hpp"

enum BindFlag
{
	DIFFUSE_MAP = 1,
	SPECULAR_MAP = 2,
	NORMAL_MAP = 4,
	OPACITY_MAP = 8
};

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
	void bind(char bindFlags);
	void unbind(char bindFlags);
};