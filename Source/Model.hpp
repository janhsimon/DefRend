#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm.hpp>
#include <vector>

class Model : public Transform
{
private:
	static Assimp::Importer importer;
	
	GLuint VBO, VAO;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	std::string isolateFilename(const std::string &filename);
	bool parseMaterials(const aiScene *model, const std::string &filename);
	bool parseVertices(const aiScene *model, std::vector<Vertex> *vertices);

public:
	Model(glm::vec3 position);
	~Model();

	bool load(const std::string &filename);
	void render(bool bindMaterials);
};