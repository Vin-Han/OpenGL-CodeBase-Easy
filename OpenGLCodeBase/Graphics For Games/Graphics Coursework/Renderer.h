# pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/HeightMap.h"
#include "EnjoyColladaMesh.h"
#include "GameObject.h"
#include "AssimpMesh.h"
#include "ColladaObject.h"

#include <algorithm> 
#include <time.h>
#include <stack>


class Renderer : public OGLRenderer {
public:

	Renderer(Window& parent);
	virtual ~Renderer(void) {}

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	
	void AssimPOBJLoad();//useless now
	void ColladaOBJLoad();//using now
protected:



//shader list
	Shader* baseShader;
	Shader* skyboxShader;
	Shader* shadowShader;

//object list
	ColladaObject* testObject;
	ColladaObject* secObject;


	vector<ColladaObject*> colladaObjectArray;

//loading
	void LoadOthers();
//rendering
	void RenderFrame();
	void RenderSkybox();
	void RenderShadow();

//others
	Camera* freeCamera;
	Light* light;
	//sky box
	GLuint cubeMap;
	Mesh* cubeMesh;
	//shadow
	GLuint shadowTex;
	GLuint shadowFBO;
	Matrix4 shadowMatrix;
	vector<GameObject*> objectArray;
};
