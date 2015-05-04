#include "TextureManager.hpp"
#include "Util.hpp"

#include <assert.h>

std::vector<Texture*> TextureManager::textures;

const std::string TextureManager::DEFAULT_DIFFUSE_TEXTURE_FILENAME = "Textures\\DefaultDiffuse.tga";
const std::string TextureManager::DEFAULT_NORMAL_TEXTURE_FILENAME = "Textures\\DefaultNormal.tga";
const std::string TextureManager::DEFAULT_SPECULAR_TEXTURE_FILENAME = "Textures\\DefaultSpecular.tga";
const std::string TextureManager::DEFAULT_OPACITY_TEXTURE_FILENAME = "Textures\\DefaultOpacity.tga";

Texture* TextureManager::refTexture(std::string filename)
{
	for (Texture *t : textures)
	{
		if (filename.compare(t->getFilename()) == 0)
		{
			t->setRefCount(t->getRefCount() + 1);
			return t;
		}
	}

	Texture *newTexture;
	
	if (!Util::checkMemory(newTexture = new Texture()))
		return nullptr;
	
	if (!newTexture->load(filename))
		return nullptr;

	textures.push_back(newTexture);

	return newTexture;
}

void TextureManager::unrefTexture(Texture *texture)
{
	assert(texture);

	if (textures.size() <= 0)
		return;

	for (std::vector<Texture*>::iterator i = textures.begin(); i != textures.end(); ++i)
	{
		if (*i == texture)
		{
			texture->setRefCount(texture->getRefCount() + 1);

			if (texture->getRefCount() <= 0)
			{
				textures.erase(i);
				delete texture;
			}

			return;
		}
	}
}

std::string TextureManager::getDefaultDiffuseTextureFilename()
{
	return DEFAULT_DIFFUSE_TEXTURE_FILENAME;
}

std::string TextureManager::getDefaultNormalTextureFilename()
{
	return DEFAULT_NORMAL_TEXTURE_FILENAME;
}

std::string TextureManager::getDefaultSpecularTextureFilename()
{
	return DEFAULT_SPECULAR_TEXTURE_FILENAME;
}

std::string TextureManager::getDefaultOpacityTextureFilename()
{
	return DEFAULT_OPACITY_TEXTURE_FILENAME;
}