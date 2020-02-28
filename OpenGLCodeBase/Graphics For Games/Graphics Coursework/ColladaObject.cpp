#include "ColladaObject.h"

void ColladaObject::ToDraw(Shader* soloShader){
	modelMatrix = nowMatrix.UpdateModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(soloShader->GetProgram(), "modelMatrix"), 1, false, (float*)&(modelMatrix));
	glUniform4fv(glGetUniformLocation(soloShader->GetProgram(), "objectColor"), 1, (float*)&color);
	colladaMesh->EnjoyColladaMesh::Draw();
}


void ColladaObject::SetPosition(Vector3 newPosition){
	nowMatrix.translation = Matrix4::Translation(newPosition);
}

void ColladaObject::SetScale(Vector3 newScale) {
	Matrix4 temp = Matrix4::Scale(newScale);
	nowMatrix.scale = temp;
}

void ColladaObject::SetRotation(float degree, Vector3 axis){
	nowMatrix.rotation = Matrix4::Rotation(degree, axis);
}


bool ColladaObject::IsActive(){
	return isActive;
}

void ColladaObject::AddPosition(Vector3 newPosition){
	nowMatrix.translation = nowMatrix.translation* Matrix4::Translation(newPosition);
}

void ColladaObject::AddRotation(float degree, Vector3 axis){
	nowMatrix.rotation = Matrix4::Rotation(degree, axis)* nowMatrix.rotation;
}

void ColladaObject::SetTexture(string newTexture){
	for (Mesh* temp : colladaMesh->ChildMeshes) {
		string tempS = TEXTUREDIR + newTexture;
		const char* tempC = tempS.c_str();
		temp->SetTexture(SOIL_load_OGL_texture(
			tempC, SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		if (!temp->GetTexture()) { return; }
	}
}

void ColladaObject::SetActive(bool activeState){
	isActive = activeState;
}


void ColladaObject::ReSetTransform() {
	modelMatrix = originMatrix.UpdateModelMatrix();
}

void ColladaObject::SetOriginalPosition(Vector3 newPosition){
	originMatrix.translation = Matrix4::Translation(newPosition);
	ReSetTransform();
}

void ColladaObject::SetOriginalRotation(float degree, Vector3 axis){
	originMatrix.rotation = Matrix4::Rotation(degree, axis);
	ReSetTransform();
}

void ColladaObject::SetOriginalScale(Vector3 newScale){
	originMatrix.scale = Matrix4::Scale(newScale);
	ReSetTransform();
}



void ColladaObject::SetColor(Vector4 newColor){
	color = newColor;
}

void ColladaObject::ResetMesh(EnjoyColladaMesh* newMesh){
	colladaMesh = newMesh;
	meshCount = colladaMesh->ChildMeshes.size();
}
