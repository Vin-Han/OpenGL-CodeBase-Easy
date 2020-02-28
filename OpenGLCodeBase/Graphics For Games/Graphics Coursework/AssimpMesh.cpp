#include "AssimpMesh.h"

AssimpMesh* AssimpMesh::LoadDaeMesh(const string& filePath)
{
	AssimpMesh* newMesh = new AssimpMesh();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return nullptr;
	}
	newMesh->ProcessNode(scene->mRootNode, scene);
	return  newMesh;
}

void AssimpMesh::Draw()
{
	Mesh::Draw();
	for (auto* item : Children)
	{
		item->Draw();
	}
}

const std::vector<AssimpMesh*>& AssimpMesh::GetChildren()
{
	return Children;
}

AssimpMesh::AssimpMesh()
{
	Mesh();
}

void AssimpMesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		if (!IsInitialized)
		{
			*this = *ProcessMesh(mesh, scene);
			IsInitialized = true;
		}
		else
		{
			Children.push_back(ProcessMesh(mesh, scene));
		}
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

AssimpMesh* AssimpMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	AssimpMesh* newMesh = new AssimpMesh();

	newMesh->numVertices = mesh->mNumVertices;
	newMesh->vertices = new Vector3[mesh->mNumVertices];
	newMesh->normals = new Vector3[mesh->mNumVertices];
	newMesh->textureCoords = new Vector2[mesh->mNumVertices];

	newMesh->numIndices = mesh->mNumFaces * 3;
	newMesh->indices = new unsigned int[newMesh->numIndices];
	//newMesh->colors = new Vector4[newMesh->numIndices];
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		newMesh->vertices[i].x = mesh->mVertices[i].x;
		newMesh->vertices[i].y = mesh->mVertices[i].y;
		newMesh->vertices[i].z = mesh->mVertices[i].z;

		newMesh->normals[i].x = mesh->mNormals[i].x;
		newMesh->normals[i].y = mesh->mNormals[i].y;
		newMesh->normals[i].z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			newMesh->textureCoords[i].x = mesh->mTextureCoords[0][i].x;
			newMesh->textureCoords[i].y = mesh->mTextureCoords[0][i].y;
		}
		//newMesh->colors[i] = Vector4(1, 1, 1, 1);
	}
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			newMesh->indices[i * 3 + j] = face.mIndices[j];
		}
	}
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	aiString filename;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &filename);
	string filename_str(filename.C_Str());
	string dir(TEXTUREDIR);
	string path = (dir + filename_str).c_str();
	newMesh->texture = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	newMesh->BufferData();
	return newMesh;
}
