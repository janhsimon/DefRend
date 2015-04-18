#include "Error.hpp"
#include "Model.hpp"

#include <assert.h>
#include <assimp/postprocess.h>

Assimp::Importer Model::importer = Assimp::Importer();

bool Model::parseMaterials(const aiScene *model)
{
	for (unsigned int materialIndex = 0; materialIndex < model->mNumMaterials; ++materialIndex)
	// loop through each material in the model
	{
		Material *newMaterial = new Material();

		if (!newMaterial->load(model->mMaterials[materialIndex]))
			return false;

		materials.push_back(newMaterial);
	}

	return true;
}

void Model::parseVertices(const aiScene *model, std::vector<Vertex> *vertices)
{
	for (unsigned int meshIndex = 0; meshIndex < model->mNumMeshes; ++meshIndex)
	// loop through each mesh in the model
	{
		Mesh *newMesh = new Mesh();

		std::vector<Vertex> meshVertices = newMesh->load(model->mMeshes[meshIndex]);

		for (Vertex v : meshVertices)
			vertices->push_back(v);

		meshes.push_back(newMesh);
	}
}

Model::~Model()
{
	for (Material *m : materials)
		delete m;

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

bool Model::load(const std::string &filename)
{
	const aiScene *model = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!model)
	// if there was an issue loading the model
	{
		Error::report("Error", "Failed to load model \"" + filename + "\": " + importer.GetErrorString());
		return false;
	}

	if (!parseMaterials(model))
		return false;

	std::vector<Vertex> vertices;
	parseVertices(model, &vertices);

	// generate and bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	// generate and bind a VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// set vertex attribute array pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);								// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 3));	// tex coord uvs
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 5));	// normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 8));	// tangent

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate VBO and VAO for model \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void Model::render(bool bindMaterials)
{
	glBindVertexArray(VAO);

	unsigned int vertex = 0;
	for (Mesh *m : meshes)
	{
		if (bindMaterials)
		{
			assert(m->getMaterialIndex() >= 0 && m->getMaterialIndex() < materials.size());
			Material *material = materials[m->getMaterialIndex()];
			assert(material);
			material->bind();
		}

		m->render(vertex);

		vertex += m->getVertexCount();
	}
}

glm::mat4 Model::getWorldMatrix()
{
	return worldMatrix;
}

void Model::setWorldMatrix(glm::mat4 &worldMatrix)
{
	this->worldMatrix = worldMatrix;
}