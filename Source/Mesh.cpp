#include "Mesh.hpp"

#include <assert.h>
#include <SDL_opengl.h>

std::vector<Vertex> Mesh::load(aiMesh *mesh)
{
	std::vector<Vertex> vertices;

	// also take note of the material index
	materialIndex = mesh->mMaterialIndex;

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
			aiVector3D tangent = mesh->mTangents[face->mIndices[i]];

			// add this vertex to our list
			Vertex newVertex = { pos.x, pos.y, pos.z, uv.x, uv.y, normal.x, normal.y, normal.z, tangent.x, tangent.y, tangent.z };
			vertices.push_back(newVertex);
		}
	}

	vertexCount = mesh->mNumFaces * 3;

	return vertices;
}

void Mesh::render(unsigned int firstVertex)
{
	glDrawArrays(GL_TRIANGLES, firstVertex, vertexCount);
}

unsigned int Mesh::getVertexCount()
{
	return vertexCount;
}

unsigned int Mesh::getMaterialIndex()
{
	return materialIndex;
}