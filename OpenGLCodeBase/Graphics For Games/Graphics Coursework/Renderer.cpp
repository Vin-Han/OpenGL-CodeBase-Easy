#include"Renderer.h"


//load sence
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	auto LoadShader = [this](Shader** object, string verName, string fragName) {
		*object = new Shader(SHADERDIR + verName, SHADERDIR + fragName);
		if (!(*object)->LinkProgram()) {
			return;
		}
	};
	LoadShader(&baseShader, "BaseVertex.glsl", "BaseFragment.glsl");
	LoadShader(&skyboxShader, "SkyboxVertex.glsl", "SkyboxFragment.glsl");
	LoadShader(&shadowShader, "ShadowVertex.glsl", "ShadowFragment.glsl");

	AssimPOBJLoad();
	ColladaOBJLoad();

	freeCamera = new Camera(0.0f, 50.0f, Vector3(0, 0, 0));
	light = new Light(Vector3(500.0f, 500.0f, 500.0f), Vector4(1, 1, 1, 1), 100.0f);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	LoadOthers();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

void Renderer::AssimPOBJLoad() {

	auto LoadGameObject = [this](GameObject** object, Mesh* mesh) {
		*object = new GameObject(mesh);
		objectArray.push_back(*object);
	};

	auto LoadMesh = [this](AssimpMesh** mesh, string meshName, string texName = "") {
		(*mesh) = new AssimpMesh(MESHDIR + meshName);
		if (texName != "") {
			string tempS = TEXTUREDIR + texName;
			const char* tempC = tempS.c_str();
			(*mesh)->SetTexture(SOIL_load_OGL_texture(
				tempC, SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
			if (!(*mesh)->GetTexture()) { return; }
			SetTextureRepeating((*mesh)->GetTexture(), true);
		}

	};
}

void Renderer::LoadOthers() {

	//skybox
	cubeMesh = Mesh::GenerateQuad();
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "grave_ft.tga", TEXTUREDIR "grave_bk.tga",
		TEXTUREDIR "grave_up.tga", TEXTUREDIR "grave_dn.tga",
		TEXTUREDIR "grave_rt.tga", TEXTUREDIR "grave_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	SetTextureRepeating(cubeMesh->GetTexture(), true);

	//shadow
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ColladaOBJLoad() {
	//load objects
	auto LoadColladaOBJ = [this](ColladaObject** OBJName, string mesh, char* textureName = NULL) {
		string tempS = MESHDIR + mesh;
		const char* tempC = tempS.c_str();
		(*OBJName) = new ColladaObject(mesh);
		if (textureName != NULL)(*OBJName)->SetTexture(textureName);
		colladaObjectArray.push_back(*OBJName);
	};

	LoadColladaOBJ(&testObject, "TestLevel.dae", "tex_MinigolfPack.png");
	LoadColladaOBJ(&secObject, "TestLevel2.dae", "tex_MinigolfPack.png");

	//preset of objects
	secObject->SetPosition(Vector3(3, 3, 3));
	testObject->SetPosition(Vector3(-3, -3, -3));
}


//update
void Renderer::UpdateScene(float msec) {
	freeCamera->UpdateCamera(msec);
	viewMatrix = freeCamera->BuildViewMatrix();

	secObject->AddRotation(1,Vector3(1,0,0));
	testObject->AddRotation(-1, Vector3(0, 1, 0));
}


//render sence
void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glBindVertexArray(0);
	RenderSkybox();

	RenderFrame();

	SwapBuffers();
}

void Renderer::RenderFrame(){

	glUseProgram(baseShader->GetProgram());
	SetCurrentShader(baseShader);
	UpdateShaderMatrix();
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, (float*)&shadowMatrix);
	SetShaderLight(*light);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "camPos"), 1, false, (float*)&(freeCamera->GetPosition()));

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	for (ColladaObject* temp : colladaObjectArray){
		if (temp->IsActive()){
			temp->ToDraw(baseShader);
		}
	}
	


	glUseProgram(0);
}

//unusing now
void Renderer::RenderShadow() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 2048, 2048);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glCullFace(GL_FRONT);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 1, 0));

	shadowMatrix = biasMatrix * (projMatrix * viewMatrix);
	auto objectBegin = objectArray.begin();
	UpdateShaderMatrix();
	for (; objectBegin != objectArray.end(); objectBegin++) {
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&((*objectBegin)->GetModelMatrix()));
		(*objectBegin)->GetMesh()->Draw();
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	viewMatrix = freeCamera->BuildViewMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
} 

void Renderer::RenderSkybox() {
	glUseProgram(skyboxShader->GetProgram());
	SetCurrentShader(skyboxShader);

	glDepthMask(GL_FALSE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	UpdateShaderMatrices();

	cubeMesh->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}





