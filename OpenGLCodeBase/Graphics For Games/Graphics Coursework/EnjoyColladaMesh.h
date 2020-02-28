#pragma once

#include "EnjoyCollada.h"
#include "../../nclgl/Mesh.h"

class EnjoyColladaMesh: public EnjoyCollada
{
public:
	EnjoyColladaMesh(const char* path);
	~EnjoyColladaMesh();
	void Draw();

	std::vector<Mesh*> ChildMeshes;
};

