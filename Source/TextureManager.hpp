#pragma once

#include "Texture.hpp"

#include <string>
#include <vector>

class TextureManager
{
private:
	static std::vector<Texture*> textures;

	static const std::string DEFAULT_DIFFUSE_TEXTURE_FILENAME;
	static const std::string DEFAULT_NORMAL_TEXTURE_FILENAME;
	static const std::string DEFAULT_OPACITY_TEXTURE_FILENAME;

public:
	static Texture* refTexture(std::string filename);
	static void unrefTexture(Texture *texture);

	static std::string getDefaultDiffuseTextureFilename();
	static std::string getDefaultNormalTextureFilename();
	static std::string getDefaultOpacityTextureFilename();
};