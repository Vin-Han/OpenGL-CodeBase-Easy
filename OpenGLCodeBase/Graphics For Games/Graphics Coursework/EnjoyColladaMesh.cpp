#include "EnjoyColladaMesh.h"

EnjoyColladaMesh::EnjoyColladaMesh(const char* path): EnjoyCollada(path)
{
	for (auto item : meshes)
	{
		Mesh* newMesh = new Mesh();
		Vector3* vertices = new Vector3[item.vertices.size()];
		Vector3* normals = new Vector3[item.normals.size()];
		Vector2* texcoords = new Vector2[item.texcoords.size()];
		Vector4* colors = new Vector4[item.vertices.size()];
		for (int i = 0; i < item.vertices.size(); i++)
		{
			vertices[i].x = item.vertices[i].x;
			vertices[i].y = item.vertices[i].y;
			vertices[i].z = item.vertices[i].z;

			normals[i].x = item.normals[i].x;
			normals[i].y = item.normals[i].y;
			normals[i].z = item.normals[i].z;

			texcoords[i].x = item.texcoords[i].x;
			texcoords[i].y = item.texcoords[i].y;
			
			colors[i] = Vector4(1, 1, 1, 1);
		}

		newMesh->SetVertices(vertices);
		newMesh->SetNormals(normals);
		newMesh->SetTexcoords(texcoords);
		newMesh->SetColors(colors);
		newMesh->SetNumIndices(item.indices.size());
		newMesh->SetNumVertices(item.vertices.size());
		newMesh->BufferData();

		ChildMeshes.push_back(newMesh);
	}
}

EnjoyColladaMesh::~EnjoyColladaMesh()
{
	for (auto* item : ChildMeshes)
	{
		delete item;
	}
}

void EnjoyColladaMesh::Draw()
{
	for (auto* item : ChildMeshes)
	{
		item->Draw();
	}
}
