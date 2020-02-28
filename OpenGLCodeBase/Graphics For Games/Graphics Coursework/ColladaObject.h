#pragma once
#pragma once
#include "../../nclgl/Mesh.h"
#include "../../nclgl/Matrix4.h"
#include "../../nclgl/Shader.h"
#include "EnjoyColladaMesh.h"

struct TSRBag {
	Matrix4 translation;
	Matrix4 scale;
	Matrix4 rotation;
	Matrix4 UpdateModelMatrix() {
		return translation* scale* rotation;
	}
	void ReSet() {
		translation = Matrix4::Translation(Vector3(0, 0, 0));
		scale = Matrix4::Scale(Vector3(1, 1, 1));
		rotation = Matrix4::Rotation(0, Vector3(0, 0, 0));
	}
};

class ColladaObject{
public:
	ColladaObject(string meshName) {

		originMatrix.ReSet();
		nowMatrix.ReSet();

		string tempS = MESHDIR + meshName;
		const char* tempC = tempS.c_str();
		colladaMesh = new EnjoyColladaMesh(tempC);

		modelMatrix.ToIdentity();
		meshCount = colladaMesh->ChildMeshes.size();
		isActive = true;
		color = Vector4(1,1,1,1);
	}

	void ToDraw(Shader* soloShader);

	void SetPosition(Vector3 newPosition);
	void SetRotation(float degree, Vector3 axis);
	void SetScale(Vector3 newScale);

	void AddPosition(Vector3 newPosition);
	void AddRotation(float degree, Vector3 axis);



	void SetOriginalPosition(Vector3 newPosition);
	void SetOriginalRotation(float degree, Vector3 axis);
	void SetOriginalScale(Vector3 newScale);
	void ReSetTransform();

	void SetColor(Vector4 newColor);
	void ResetMesh(EnjoyColladaMesh* newMesh);

	void SetTexture(string newTexture);

	bool IsActive();
	void SetActive(bool activeState);

protected:
	EnjoyColladaMesh* colladaMesh;
	Vector4 color;
	Matrix4 modelMatrix;
	int meshCount;

	bool isActive;

	TSRBag originMatrix;
	TSRBag nowMatrix;
};

