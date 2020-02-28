#pragma once
#include "../../nclgl/Mesh.h"
#include "../../nclgl/common.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpMesh: public Mesh
{
public:
	static AssimpMesh* LoadDaeMesh(const string& filePath);
	AssimpMesh(const string& filePath)
	{
		*this = *LoadDaeMesh(filePath);
	}

	virtual void Draw();

	const std::vector<AssimpMesh*>& GetChildren();

protected:
	std::vector<AssimpMesh*> Children;

private:
	AssimpMesh();
	void ProcessNode(aiNode* node, const aiScene* scene);
	AssimpMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool IsInitialized = false;
};

