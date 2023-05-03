#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "ShaderProgram.h"
#include "Mesh.h"

#include <string>
#include <vector>
#include <iostream>

class Model
{
public:
	Model(const std::string& path)
	{
		loadModel(path);
	}

	void Draw(ShaderProgram& shader);

public:
	/* 模型数据 */
	std::vector<Mesh> meshes;
	std::vector<MyTexture> textures_loaded;
	std::string directory;
	/* 函数 */
	void loadModel(const std::string& path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);;
	std::vector<MyTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};