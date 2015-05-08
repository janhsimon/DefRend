#pragma once

#include "ITransform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm.hpp>
#include <vector>

class Model : public ITransform
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