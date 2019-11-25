#include "ModuleTexture.h"

#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>
#include <IMGUI/imgui.h>

using namespace std;

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	LOG("Init Image library");
	ilInit();
	iluInit();
	ilutInit();

	return true;
}

bool ModuleTexture::CleanUp()
{
	for (unsigned int i = 0; i < textures_loaded.size(); ++i)
	{
		glDeleteTextures(1, &textures_loaded[i].id);
	}
	textures_loaded.clear();

	return true;
}

void ModuleTexture::LoadTextureForModels(const char * path, const string directory, Texture &texture)
{
	std::string filepath = directory;
	filepath.append(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	LOG("Trying to load texture as specified in fbx file: %s.", filepath.c_str());

	//Loading image
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	bool imageLoaded = ilLoadImage(filepath.c_str());
	
	if (!imageLoaded)
	{
		size_t lastSlash = filepath.find_last_of('\\');
		filepath = directory + filepath.substr(lastSlash, string::npos);
		LOG("Trying to load texture from same directory as fbx: %s.", filepath.c_str());
		imageLoaded = ilLoadImage(filepath.c_str());
	}

	if (!imageLoaded)
	{
		filepath = "../Textures/";
		filepath.append(path);
		LOG("Trying to load texture from Textures folder: %s.", filepath.c_str());
		imageLoaded = ilLoadImage(filepath.c_str());
	}

	if (!imageLoaded)
	{
		LOG("Cannot find texture.");
		return;
	}

	bool isRGB = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if (!isRGB)
	{
		ILenum error = ilGetError();
		LOG("Error converting image to rgb: %s - %s", std::to_string(error), iluErrorString(error));
		return;
	}

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	ILubyte* data = ilGetData();

	if (data)
	{
		//Fill texture
		texture.width = ilGetInteger(IL_IMAGE_WIDTH);
		texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture.format = ilDetermineType(path);
		texture.data = data;
		texture.path = filepath.c_str();

		//Binding texture and generating mipmaps
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("Failed to load texture");
	}
	//Filtering and Wrapping texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Delete image
	iluDeleteImage(image);

	texture.id = textureID;

	return;
}

vector<Texture> ModuleTexture::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName, string directory)
{
	textures_loaded.clear();

	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		aiTextureMapping mapping = aiTextureMapping_UV;
		mat->GetTexture(type, i, &str, &mapping, 0);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			Texture texture;
			LoadTextureForModels(str.C_Str(), directory, texture);
			texture.type = typeName;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}


	}
	return textures;
}

void ModuleTexture::ShowTextureProperties()
{
	ImGui::Text("Placeholder. No options for Textures are enabled.");
}
