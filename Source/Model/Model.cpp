#include <assert.h>
#include <assimp\postprocess.h>

#include "Model.hpp"
#include "..\Util\Error.hpp"
#include "..\Util\Util.hpp"

Assimp::Importer Model::importer = Assimp::Importer();

std::string Model::isolateFilename(const std::string &filename)
{
	std::string isolatedFilename = filename;
	isolatedFilename = isolatedFilename.substr(isolatedFilename.find_last_of("\\") + 1, isolatedFilename.length());
	isolatedFilename = isolatedFilename.substr(0, isolatedFilename.find_last_of("."));
	return isolatedFilename;
}

bool Model::parseMaterials(const aiScene *model, const std::string &filename)
{
	for (unsigned int materialIndex = 0; materialIndex < model->mNumMaterials; ++materialIndex)
	// loop through each material in the model
	{
		Material *newMaterial = new Material();

		if (!newMaterial->load(model->mMaterials[materialIndex], filename))
			return false;

		materials.push_back(newMaterial);
	}

	return true;
}

bool Model::parseVertices(const aiScene *model, std::vector<Vertex> *vertices)
{
	for (unsigned int meshIndex = 0; meshIndex < model->mNumMeshes; ++meshIndex)
	// loop through each mesh in the model
	{
		Mesh *newMesh;
		
		if (!Util::checkMemory(newMesh = new Mesh()))
			return false;

		std::vector<Vertex> meshVertices = newMesh->load(model->mMeshes[meshIndex]);

		for (Vertex v : meshVertices)
			vertices->push_back(v);

		meshes.push_back(newMesh);
	}

	return true;
}

Model::Model(glm::vec3 position) : Transform(position)
{
	
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

	if (!parseMaterials(model, isolateFilename(filename)))
		return false;

	std::vector<Vertex> vertices;
	
	if (!parseVertices(model, &vertices))
		return false;

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

void Model::render(char bindFlags)
{
	glBindVertexArray(VAO);

	unsigned int vertex = 0;
	for (Mesh *m : meshes)
	{
		assert(m->materialIndex >= 0 && m->materialIndex < materials.size());
		Material *material = materials[m->materialIndex];
		assert(material);

		material->bind(bindFlags);
		m->render(vertex);
		material->unbind(bindFlags);

		vertex += m->vertexCount;
	}
}