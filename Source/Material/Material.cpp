#include <glew.h>
#include <SDL_opengl.h>

#include "Material.hpp"
#include "..\Util\Error.hpp"

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
		textureType = aiTextureType_SPECULAR;
		specularMap = nullptr;
		typeDesc = "specular";
	}
	else if (slot == 2)
	{
		textureType = aiTextureType_HEIGHT;
		normalMap = nullptr;
		typeDesc = "normal";
	}
	else if (slot == 3)
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
			specularMap = TextureManager::refTexture(TextureManager::getDefaultSpecularTextureFilename());

			if (!specularMap)
				return false;
		}
		else if (slot == 2)
		{
			normalMap = TextureManager::refTexture(TextureManager::getDefaultNormalTextureFilename());

			if (!normalMap)
				return false;
		}
		else if (slot == 3)
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
		specularMap = TextureManager::refTexture(completeMaterialFilename);

		if (!specularMap)
			return false;
	}
	else if (slot == 2)
	{
		normalMap = TextureManager::refTexture(completeMaterialFilename);

		if (!normalMap)
			return false;
	}
	else if (slot == 3)
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

	if (specularMap)
		TextureManager::unrefTexture(specularMap);
	
	if (normalMap)
		TextureManager::unrefTexture(normalMap);

	if (opacityMap)
		TextureManager::unrefTexture(opacityMap);
}

bool Material::load(aiMaterial *material, const std::string &modelFilename)
{
	parseName(material);

	for (int i = 0; i < 4; ++i)
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
	
	if (specularMap)
		specularMap->bind(GL_TEXTURE1);

	if (normalMap)
		normalMap->bind(GL_TEXTURE2);

	if (opacityMap)
		opacityMap->bind(GL_TEXTURE3);
}

void Material::unbind()
{
	if (diffuseMap)
		diffuseMap->unbind(GL_TEXTURE0);

	if (specularMap)
		specularMap->unbind(GL_TEXTURE1);

	if (normalMap)
		normalMap->unbind(GL_TEXTURE2);

	if (opacityMap)
		opacityMap->unbind(GL_TEXTURE3);
}