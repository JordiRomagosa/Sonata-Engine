#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "AssimpLog.h"

#include <Assimp/cimport.h>
#include <Assimp/postprocess.h>
#include <MathGeoLib/MathGeoLib.h>
#include <IMGUI/imgui.h>
#include <Assimp/Logger.hpp>
#include <Assimp/DefaultLogger.hpp>

using namespace std;

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	return true;
}

void ModuleModelLoader::Draw(unsigned const int program) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}

void ModuleModelLoader::loadModel(const string path)
{
	if (isModelLoaded)
		emptyScene();

	LOG("Importing model \n");

	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::DefaultLogger::create("", Assimp::Logger::NORMAL);
	Assimp::DefaultLogger::get()->attachStream(new AssimpLog(), severity);

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
	isModelLoaded = true;

	Assimp::DefaultLogger::kill();

	App->camera->FocusCameraOnModel();
}

void ModuleModelLoader::loadTexture(const string path)
{
	if (!isModelLoaded)
		return;

	LOG("Importing texture \n");

	Texture texture;
	directory = "";
	
	App->texture->LoadTextureForModels(path.c_str(), directory, texture);
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i]->textures.clear();
		meshes[i]->textures.push_back(texture);
	}
}

void ModuleModelLoader::processNode(aiNode * node, const aiScene * scene)
{
	LOG("For each mesh located on the current node, processing meshes.")
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		LOG("Processing mesh: %d of %d.", i + 1, node->mNumMeshes);
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(new Mesh(processMesh(mesh, scene)));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModuleModelLoader::processMesh(aiMesh * mesh, const aiScene * scene)
{
	//Filling data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	std::vector<Texture> textures;
	AABB boundingBox;

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

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = App->texture->loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse", directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = App->texture->loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular", directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = App->texture->loadMaterialTextures(material,
			aiTextureType_HEIGHT, "texture_normal", directory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = App->texture->loadMaterialTextures(material,
			aiTextureType_AMBIENT, "texture_height", directory);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	if (mesh->mNumVertices > 0)
	{
		boundingBox.SetNegativeInfinity();
		boundingBox.Enclose((float3*)mesh->mVertices, mesh->mNumVertices);
	}
	
	return Mesh(vertices, indices, textures, boundingBox);
}

string ModuleModelLoader::computeDirectory(const string path) const
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
			return path.substr(0, path.find_last_of('/') + 1);
	}
	size_t doubleRightSlash = path.find_last_of("//");
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
			return path.substr(0, path.find_last_of("//") + 1);
	}

	size_t doubleLeftSlash = path.find_last_of("\\");
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
			return path.substr(0, path.find_last_of("\\") + 1);
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

AABB ModuleModelLoader::GetModelAABB() const
{
	AABB totalBoundingBox;
	if (isModelLoaded)
	{
		totalBoundingBox = meshes[0]->boundingBox;

		for (unsigned i = 1; i < meshes.size(); i++)
		{
			totalBoundingBox.Enclose(meshes[i]->boundingBox);
		}
	}

	return totalBoundingBox;
}

float3 ModuleModelLoader::GetModelCenter() const
{
	if (isModelLoaded)
	{
		float3 minPoint = meshes[0]->boundingBox.minPoint;
		float3 maxPoint = meshes[0]->boundingBox.maxPoint;

		for (unsigned i = 1; i < meshes.size(); i++)
		{
			float3 newMinPoint = meshes[i]->boundingBox.minPoint;
			float3 newMaxPoint = meshes[i]->boundingBox.maxPoint;

			minPoint = math::Min(minPoint, newMinPoint);
			maxPoint = math::Max(maxPoint, newMaxPoint);
		}

		float3 center = maxPoint.Add(minPoint);
		center = center.Div(2);

		return center;
	}

	return float3(0, 0, 0);
}

void ModuleModelLoader::ShowModelProperties() const
{
	if (ImGui::CollapsingHeader("Transformation")) //mockup, since model matrix can't change
	{
		ImGui::Text("Model Translation: x:%.3f y:%.3f z:%.3f", 0.0, 0.0, 0.0);
		ImGui::Text("Model Rotation: x:%.3f y:%.3f z:%.3f", 0.0, 0.0, 0.0);
		ImGui::Text("Model Scale: x:%.3f y:%.3f z:%.3f", 1.0, 1.0, 1.0);
	}
	
	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::Text("Number Meshes: %d", meshes.size());

		int triCount = 0;

		if (meshes.size() > 0)
			for (unsigned i = 0; i < meshes.size(); i++)
				triCount += meshes[i]->indices.size();
		ImGui::Text("Number Triangles: %d", triCount/3);
		ImGui::Text("Number Indexes: %d", triCount);

		int verCount = 0;

		if (meshes.size() > 0)
			for (unsigned i = 0; i < meshes.size(); i++)
				verCount += meshes[i]->vertices.size();
		ImGui::Text("Number Vertices: %d", verCount);
	}
	
	if (ImGui::CollapsingHeader("Texture"))
	{
		vector<Texture> textures;
		for (unsigned i = 0; i < meshes.size(); i++)
		{
			for (unsigned c = 0; c < meshes[i]->textures.size(); c++)
			{
				if (textures.size() == 0)
					textures.push_back(meshes[i]->textures[c]);
				else
					for (vector<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
						if (it->path != meshes[i]->textures[c].path)
						{
							textures.push_back(meshes[i]->textures[c]);
							break;
						}
							
			}
		}
		ImGui::Text("Textures loaded: %d", textures.size());

		for (vector<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			ImGui::Separator();
			ImGui::Text("Texture Path: %s", it->path.c_str());
			ImGui::Text("Texture Size: %dx%d px", it->width, it->height);
			ImGui::Text("Texture Type: %s", it->type.c_str());

			float proportion = (float)it->width / (float)it->height;
			ImGui::Image((void*)(intptr_t)it->id, ImVec2(200, 200 * proportion), ImVec2(0,1), ImVec2(1,0));
		}
	}
}
