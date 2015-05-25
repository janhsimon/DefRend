#pragma once

#include <string>
#include <vector>

#include "Texture.hpp"

class TextureManager
{
private:
	static std::vector<Texture*> textures;

	static const std::string DEFAULT_DIFFUSE_TEXTURE_FILENAME;
	static const std::string DEFAULT_NORMAL_TEXTURE_FILENAME;
	static const std::string DEFAULT_OPACITY_TEXTURE_FILENAME;
	static const std::string DEFAULT_SPECULAR_TEXTURE_FILENAME;

public:
	static Texture* refTexture(std::string filename);
	static void unrefTexture(Texture *texture);

	const static std::string &getDefaultDiffuseTextureFilename();
	const static std::string &getDefaultNormalTextureFilename();
	const static std::string &getDefaultSpecularTextureFilename();
	const static std::string &getDefaultOpacityTextureFilename();
};