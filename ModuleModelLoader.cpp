#include "Globals.h"
#include "ModuleModelLoader.h"

#include <Assimp/cimport.h>
#include <Assimp/postprocess.h>
//#include <Assimp/Importer.hpp>

using namespace std;

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	loadModel("../Game/BakerHouse.fbx");
	
	return true;
}

void ModuleModelLoader::Draw(unsigned int program)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}

void ModuleModelLoader::loadModel(const string path)
{
	if (isModelLoaded)
		emptyScene();

	LOG("Importing model \n");
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("ERROR ASSIMP: %s \n", aiGetErrorString());
		return;
	}

	directory = computeDirectory(path);

	if (directory == "")
		return;
	processNode(scene->mRootNode, scene);
	//computeModelBoundingBox();
	isModelLoaded = true;

}

void ModuleModelLoader::processNode(aiNode * node, const aiScene * scene)
{
	LOG("For each mesh located on the current node, processing meshes.")
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			LOG("Processing mesh");
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(new Mesh(processMesh(mesh, scene)));
		}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LOG("Processing node child");
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModuleModelLoader::processMesh(aiMesh * mesh, const aiScene * scene)
{
	//Filling data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	//std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		float3 positions;
		positions.x = mesh->mVertices[i].x;
		positions.y = mesh->mVertices[i].y;
		positions.z = mesh->mVertices[i].z;
		vertex.Position = positions;

		float3 normals;
		normals.x = mesh->mNormals[i].x;
		normals.y = mesh->mNormals[i].y;
		normals.z = mesh->mNormals[i].z;
		vertex.Normal = normals;


		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			float2 texturesCoords;
			texturesCoords.x = mesh->mTextureCoords[0][i].x;
			texturesCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texturesCoords;
		}
		else
			vertex.TexCoords = float2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices of each face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	//	// 1. diffuse maps
	//	std::vector<Texture> diffuseMaps = App->texture->loadMaterialTextures(material,
	//		aiTextureType_DIFFUSE, "texture_diffuse", directory);
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	// 2. specular maps
	//	std::vector<Texture> specularMaps = App->texture->loadMaterialTextures(material,
	//		aiTextureType_SPECULAR, "texture_specular", directory);
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//	// 3. normal maps
	//	std::vector<Texture> normalMaps = App->texture->loadMaterialTextures(material,
	//		aiTextureType_HEIGHT, "texture_normal", directory);
	//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//	// 4. height maps
	//	std::vector<Texture> heightMaps = App->texture->loadMaterialTextures(material,
	//		aiTextureType_AMBIENT, "texture_height", directory);
	//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	//}

	return Mesh(vertices, indices/*, textures*/);
}

string ModuleModelLoader::computeDirectory(const string path)
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
			return path.substr(0, path.find_last_of('/') + 1);
	}
	size_t doubleRightSlash = path.find_last_of('//');
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
			return path.substr(0, path.find_last_of('//') + 1);
	}

	size_t doubleLeftSlash = path.find_last_of('\\');
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
			return path.substr(0, path.find_last_of('\\') + 1);
	}

	LOG("ERROR: Invalid path.");
	return "";
}

void ModuleModelLoader::emptyScene()
{
	for (auto mesh : meshes)
	{
		delete mesh;
	}

	meshes.clear();
	//modelBox.clear();
}