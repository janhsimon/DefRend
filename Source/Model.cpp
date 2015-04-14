#include "Error.hpp"
#include "Model.hpp"

#include <assert.h>
#include <assimp/postprocess.h>

Assimp::Importer Model::importer = Assimp::Importer();
Texture *Model::defaultTexture = nullptr;
const std::string &Model::defaultTextureFilename = "textures\\default.png";

Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// TODO: check this calls texture dtors as expected to free up memory
	textures.clear();

	if (defaultTexture)
	{
		delete defaultTexture;
		defaultTexture = nullptr;
	}
}

bool Model::load(const std::string &filename)
{
	textures.clear();
	meshFirstVertex.clear();
	meshVertexCount.clear();
	meshMaterialIndex.clear();

	if (!defaultTexture)
	{
		defaultTexture = new Texture();

		if (!defaultTexture)
		{
			Error::report("Error", "Failed to allocate memory for default texture.");
			return false;
		}

		if (!defaultTexture->load(defaultTextureFilename))
		{
			Error::report("Error", "Failed to load default texture \"" + defaultTextureFilename + "\" for model \"" + filename + "\".");
			return false;
		}
	}

	const aiScene *model = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!model)
	// if there was an issue loading the model
	{
		Error::report("Error", "Failed to load model \"" + filename + "\": " + importer.GetErrorString());
		return false;
	}

	for (unsigned int materialIndex = 0; materialIndex < model->mNumMaterials; ++materialIndex)
	// loop through each material in the model
	{
		const aiMaterial *material = model->mMaterials[materialIndex];

		std::string materialName = "<NAME_NOT_FOUND>";

		for (unsigned int i = 0; i < material->mNumProperties; ++i)
		{
			aiString retrievedName;

			if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
			{
				materialName = retrievedName.C_Str();
				break;
			}
		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0 && material->GetTextureCount(aiTextureType_AMBIENT) <= 0)
		{
			assert(defaultTexture);
			textures.push_back(defaultTexture);
			Error::report("Warning", "No diffuse or ambient texture found on global material \"" + materialName + "\" for model \"" + filename + "\".");
			continue;
		}

		aiString materialFilename;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &materialFilename, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS)
		{
			if (material->GetTexture(aiTextureType_AMBIENT, 0, &materialFilename, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS)
			{
				Error::report("Error", "Failed to retrieve a diffuse or ambient texture for model \"" + filename + "\": " + importer.GetErrorString());
				return false;
			}
		}
		
		Texture *newTexture = new Texture();
			
		if (!newTexture->load(materialFilename.C_Str()))
		{
			Error::report("Error", "Failed to load diffuse or ambient texture on a global material for model \"" + filename + "\".");
			return false;
		}

		textures.push_back(newTexture);
	}

	std::vector<Vertex> vertices;

	for (unsigned int meshIndex = 0; meshIndex < model->mNumMeshes; ++meshIndex)
	// loop through each mesh in the model
	{
		const aiMesh *mesh = model->mMeshes[meshIndex];
		assert(mesh);

		// mark the first vertex and the total vertex count of this mesh for rendering later
		meshFirstVertex.push_back(vertices.size());
		meshVertexCount.push_back(mesh->mNumFaces * 3);

		// also take note of the material index
		meshMaterialIndex.push_back(mesh->mMaterialIndex);

		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		// loop through each face in the mesh
		{
			const aiFace *face = &(mesh->mFaces[faceIndex]);
			assert(face && face->mNumIndices == 3);

			for (int i = 0; i < 3; ++i)
			// we expect triangulated faces
			{
				aiVector3D pos = mesh->mVertices[face->mIndices[i]];
				aiVector3D uv = mesh->mTextureCoords[0][face->mIndices[i]];
				aiVector3D normal = mesh->mNormals[face->mIndices[i]];

				// add this vertex to our list
				Vertex newVertex = { pos.x, pos.y, pos.z, uv.x, uv.y, normal.x, normal.y, normal.z };
				vertices.push_back(newVertex);
			}
		}
	}

	// generate and bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate and bind VAO for model \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	// enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to enable vertex attribute arrays for model \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	// generate and bind a VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to generate and bind VBO for model \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	// set vertex attribute array pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 5));

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Error::report("Error", "Failed to set vertex attribute array pointers for model \"" + filename + "\": " + Error::getOpenGLErrorString(error));
		return false;
	}

	return true;
}

void Model::render()
{
	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < meshFirstVertex.size(); ++i)
	{
		textures[meshMaterialIndex[i]]->bind(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, meshFirstVertex[i], meshVertexCount[i]);
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