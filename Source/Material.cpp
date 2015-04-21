#include "Error.hpp"
#include "Material.hpp"

#include <SDL_opengl.h>

void Material::parseName(aiMaterial *material)
{
	name = "Unknown Material";

	for (unsigned int i = 0; i < material->mNumProperties; ++i)
	// loop through each material property
	{
		aiString retrievedName;

		if (material->Get(AI_MATKEY_NAME, retrievedName) == AI_SUCCESS)
		// if this material property holds the name of our material
		{
			// store it
			name = retrievedName.C_Str();
			return;
		}
	}
}

bool Material::parseTexture(aiMaterial *material, const std::string &modelFilename, int slot)
{
	aiTextureType textureType = aiTextureType_DIFFUSE;
	std::string typeDesc = "diffuse";

	if (slot == 0)
		diffuseMap = nullptr;
	else if (slot == 1)
	{
		textureType = aiTextureType_HEIGHT;
		normalMap = nullptr;
		typeDesc = "normal";
	}
	else if (slot == 2)
	{
		textureType = aiTextureType_OPACITY;
		opacityMap = nullptr;
		typeDesc = "opacity";
	}

	if (material->GetTextureCount(textureType) <= 0)
	{
		if (slot == 0)
		{
			diffuseMap = TextureManager::refTexture(TextureManager::getDefaultDiffuseTextureFilename());

			if (!diffuseMap)
				return false;
		}
		else if (slot == 1)
		{
			normalMap = TextureManager::refTexture(TextureManager::getDefaultNormalTextureFilename());

			if (!normalMap)
				return false;
		}
		else if (slot == 2)
		{
			opacityMap = TextureManager::refTexture(TextureManager::getDefaultOpacityTextureFilename());

			if (!opacityMap)
				return false;
		}
		
		return true;
	}

	aiString materialFilename;

	if (material->GetTexture(textureType, 0, &materialFilename) != AI_SUCCESS)
	{
		Error::report("Error", "Failed to retrieve " + typeDesc + " map filename for material \"" + name + "\".");
		return false;
	}

	std::string completeMaterialFilename = "Textures\\" + modelFilename + "\\" + materialFilename.C_Str();
	
	if (slot == 0)
	{
		diffuseMap = TextureManager::refTexture(completeMaterialFilename);

		if (!diffuseMap)
			return false;
	}
	else if (slot == 1)
	{
		normalMap = TextureManager::refTexture(completeMaterialFilename);

		if (!normalMap)
			return false;
	}
	else if (slot == 2)
	{
		opacityMap = TextureManager::refTexture(completeMaterialFilename);

		if (!opacityMap)
			return false;
	}

	return true;
}

Material::~Material()
{
	if (diffuseMap)
		TextureManager::unrefTexture(diffuseMap);
	
	if (normalMap)
		TextureManager::unrefTexture(normalMap);

	if (opacityMap)
		TextureManager::unrefTexture(opacityMap);
}

bool Material::load(aiMaterial *material, const std::string &modelFilename)
{
	parseName(material);

	for (int i = 0; i < 3; ++i)
	// load all maps
	{
		if (!parseTexture(material, modelFilename, i))
			return false;
	}

	return true;
}

void Material::bind()
{
	if (diffuseMap)
		diffuseMap->bind(GL_TEXTURE0);
	
	if (normalMap)
		normalMap->bind(GL_TEXTURE1);

	if (opacityMap)
	{
		opacityMap->bind(GL_TEXTURE2);

		// set up for transparency (used by opacity maps)
		//glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		//glDepthMask(false);
	}
	else
	{
		// set up for no transparency
		//glDisable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
		//glDepthMask(true);
	}
}