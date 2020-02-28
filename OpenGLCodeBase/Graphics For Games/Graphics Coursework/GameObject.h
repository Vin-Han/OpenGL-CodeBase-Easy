#pragma once
#include "../../nclgl/Mesh.h"
#include "../../nclgl/Matrix4.h"
#include "../../nclgl/Shader.h"

class GameObject{
protected:
	Mesh* renderObject;
	Matrix4 modelMatrix;
	Vector4 objectColor;
public:
	inline GameObject(Mesh* renderObject) :renderObject(renderObject){
		modelMatrix.ToIdentity();
		objectColor = Vector4();
	}
	virtual inline ~GameObject() { delete renderObject; }

	Vector4 GetColor() { return objectColor; }
	Matrix4 GetModelMatrix() { return modelMatrix; }
	inline Mesh* GetMesh() { return renderObject; }
	inline void SetMesh(Mesh* newMesh) { renderObject = newMesh; }

public:
	void SetScale(Vector3 scale);
	void SetPosition(Vector3 scale);
	void SetColor(Vector4 newColor) { objectColor = newColor ; }
	void SetRotation(float degree, Vector3 axis);
	void AddOffset(Vector3 scale);
};

