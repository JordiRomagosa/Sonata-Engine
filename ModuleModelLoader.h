#ifndef __ModuleModelLoader_H__
#define __ModuleModelLoader_H__

#include "Module.h"
#include "Mesh.h"
#include <string>
#include <vector>
#include <Assimp/scene.h>

class float4x4;

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();

	void Draw(unsigned const int program) const;
	void loadModel(const std::string path);
	void loadTexture(const std::string path);
	AABB GetModelAABB() const;

	float3 GetModelCenter() const;
	void ShowModelProperties() const;
	 
	bool isModelLoaded = false;
	math::float4x4 model = math::float4x4::FromTRS(float3(0, 0, 0), math::float3x3::RotateX(0.0f) * math::float3x3::RotateY(0.0f), math::float3(1.0f, 1.0f, 1.0f));

private:

	std::vector<Mesh*> meshes;
	std::string directory;
	/*  Functions   */

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::string computeDirectory(const std::string path) const;
	void emptyScene();

};

#endif __ModuleModelLoader_H__