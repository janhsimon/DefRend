#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm.hpp>
#include <vector>

class Model
{
private:
	static Assimp::Importer importer;
	
	GLuint VBO, VAO;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	//glm::mat4 worldMatrix;

	std::string isolateFilename(const std::string &filename);
	bool parseMaterials(const aiScene *model, const std::string &filename);
	void parseVertices(const aiScene *model, std::vector<Vertex> *vertices);

public:
	~Model();

	bool load(const std::string &filename);
	void render(bool bindMaterials);

	//glm::mat4 getWorldMatrix();
	//void setWorldMatrix(glm::mat4 &worldMatrix);
};