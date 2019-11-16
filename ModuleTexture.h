#ifndef __ModuleModelTexture_H__
#define __ModuleModelTexture_H__

#include "Module.h"
#include <IL/ilu.h>
#include <Assimp/material.h>
#include <string>
#include <vector>

struct Texture {
	unsigned int id = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	unsigned int format = 0;
	ILubyte* data = nullptr;
	std::string type = "";
	std::string path = "";  // we store the path of the texture to compare with other textures
};

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();
	
	bool Init();
	bool CleanUp();

	void LoadTextureForModels(const char* path, const std::string directory, Texture &texture);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName, const std::string directory);

	std::vector<Texture> textures_loaded;
};

#endif __ModuleModelTexture_H__