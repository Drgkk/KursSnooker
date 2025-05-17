#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Vertices.h"
#include "Indices.h"
#include "Textures.h"
#include "Vertices.h"
#include "Model.h"
#include "Mesh.h"
#include "ImageLoaderSTB.h"

template<typename ObjectParser,
	typename MeshT>
class Model;

template<typename MeshT = Mesh,
	typename ImageLoader = ImageLoaderSTB>
class ObjectAssimpParser
{
public:
	ObjectAssimpParser();
	void Parse(std::string const& path, Model<ObjectAssimpParser, Mesh>& model);

private:
	const aiScene* scene;
	std::string directory;
	void setDirectory(std::string s);
	void processNode(aiNode *node, const aiScene *scene, Model<ObjectAssimpParser, Mesh>& model);
	MeshT processMesh(aiMesh* mesh, const aiScene* scene, Model<ObjectAssimpParser, Mesh>& model);
	Textures loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, Model<ObjectAssimpParser, Mesh>& model);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};



template<typename MeshT, typename ImageLoader>
ObjectAssimpParser<MeshT, ImageLoader>::ObjectAssimpParser()
{
}

template<typename MeshT, typename ImageLoader>
void ObjectAssimpParser<MeshT, ImageLoader>::Parse(std::string const& path, Model<ObjectAssimpParser, Mesh>& model)
{
	Assimp::Importer importer;
	this->scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	setDirectory(path.substr(0, path.find_last_of('/')));
	processNode(scene->mRootNode, scene, model);
}

template<typename MeshT, typename ImageLoader>
void ObjectAssimpParser<MeshT, ImageLoader>::setDirectory(std::string s)
{
	this->directory = s;
}

template<typename MeshT, typename ImageLoader>
void ObjectAssimpParser<MeshT, ImageLoader>::processNode(aiNode* node, const aiScene* scene, Model<ObjectAssimpParser, Mesh>& model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.GetMeshes().push_back(processMesh(mesh, scene, model));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, model);
	}
}

template<typename MeshT, typename ImageLoader>
MeshT ObjectAssimpParser<MeshT, ImageLoader>::processMesh(aiMesh* mesh, const aiScene* scene, Model<ObjectAssimpParser, Mesh>& model)
{
	Vertices vertices;
	Indices indices;
	Textures textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		VertexData vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.AddVertice(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.AddIndex(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	Textures diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", model);
	textures.InsertTexture(diffuseMaps);

	Textures specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", model);
	textures.InsertTexture(specularMaps);

	Textures normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", model);
	textures.InsertTexture(normalMaps);

	Textures heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", model);
	textures.InsertTexture(heightMaps);



	return Mesh(vertices, indices, textures);
}

template<typename MeshT, typename ImageLoader>
Textures ObjectAssimpParser<MeshT, ImageLoader>::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, Model<ObjectAssimpParser, Mesh>& model)
{
	Textures textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		auto texturesLoadedBegin = model.GetTexturesLoaded().Begin();
		auto texturesLoadedEnd = model.GetTexturesLoaded().End();
		for (auto j = texturesLoadedBegin; j != texturesLoadedEnd; j++) {
			if (std::strcmp((*j).path.data(), str.C_Str()) == 0) {
				textures.AddTexture(*j);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.AddTexture(texture);
			model.GetTexturesLoaded().AddTexture(texture);
		}
	}
	return textures;
}


template<typename MeshT, typename ImageLoader>
unsigned int ObjectAssimpParser<MeshT, ImageLoader>::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	ImageLoader imageLoader;
	imageLoader.Load(filename.c_str(), &width, &height, &nrComponents);
	if (imageLoader.GetData()) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		imageLoader.Free();
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		imageLoader.Free();
	}

	return textureID;
}
