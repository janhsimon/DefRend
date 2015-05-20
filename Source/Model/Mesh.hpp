#pragma once

#include <assimp\mesh.h>
#include <vector>

struct Vertex
{
	float positionX, positionY, positionZ;
	float u, v;
	float normalX, normalY, normalZ;
	float tangentX, tangentY, tangentZ;
};

class Mesh
{
private:
	unsigned int vertexCount;
	unsigned int materialIndex;

public:
	std::vector<Vertex> load(aiMesh *mesh);
	void render(unsigned int firstVertex);

	unsigned int getVertexCount();
	unsigned int getMaterialIndex();
};