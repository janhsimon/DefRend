#pragma once

#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm.hpp>
#include <vector>

struct Vertex
{
	float positionX, positionY, positionZ;
	float u, v;
	float normalX, normalY, normalZ;
};

class Model
{
private:
	static Assimp::Importer importer;
	static Texture *defaultTexture;
	static const std::string &defaultTextureFilename;

	GLuint VBO, VAO;
	std::vector<Texture*> textures;
	std::vector<unsigned int> meshFirstVertex;
	std::vector<unsigned int> meshVertexCount;
	std::vector<unsigned int> meshMaterialIndex;
	glm::mat4 worldMatrix;

public:
	~Model();

	bool load(const std::string &filename);
	void render();

	glm::mat4 getWorldMatrix();
	void setWorldMatrix(glm::mat4 &worldMatrix);
};