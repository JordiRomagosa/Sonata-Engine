#ifndef __Mesh_H__
#define __Mesh_H__

#include "MathGeoLib/MathGeoLib.h"
#include "ModuleTexture.h"
#include <vector>

struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};

class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	AABB boundingBox;

	/*  Functions  */
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, AABB boundingBox);
	~Mesh();
	void Draw(unsigned int program) const;

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};

#endif __Mesh_H__