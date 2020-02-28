#include "GameObject.h"

void GameObject::SetScale(Vector3 scale) {
	modelMatrix.values[0] = scale.x;
	modelMatrix.values[5] = scale.y;
	modelMatrix.values[10] = scale.z;
}

void GameObject::SetPosition(Vector3 scale) {
	modelMatrix.values[12] = scale.x;
	modelMatrix.values[13] = scale.y;
	modelMatrix.values[14] = scale.z;
}

void GameObject::AddOffset(Vector3 scale) {
	modelMatrix.values[12] += scale.x;
	modelMatrix.values[13] += scale.y;
	modelMatrix.values[14] += scale.z;
}

void GameObject::SetRotation(float degree, Vector3 axis) {
	modelMatrix = Matrix4::Rotation(degree, axis) * modelMatrix;
}