#ifndef __Mesh_H__
#define __Mesh_H__

#include <vector>
#include "MathGeoLib/MathGeoLib.h"

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
	//std::vector<Texture> textures;
	/*  Functions  */
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices/*, std::vector<Texture>& textures*/);
	~Mesh();
	void Draw(unsigned int program) const;

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};

#endif __Mesh_H__