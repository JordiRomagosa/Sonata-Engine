#ifndef __ModuleModelLoader_H__
#define __ModuleModelLoader_H__

#include "Module.h"
#include "Mesh.h"
#include <string>
#include <vector>
#include <Assimp/scene.h>

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();

	void Draw(unsigned int program);
	void loadModel(std::string path);

	bool isModelLoaded = false;

private:

	std::vector<Mesh*> meshes;
	std::string directory;
	/*  Functions   */

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::string computeDirectory(const std::string path);
	void emptyScene();

};

#endif __ModuleModelLoader_H__