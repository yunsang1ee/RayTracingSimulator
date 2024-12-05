#include "PlanetaryScene.h"
#include <ysObject.h>
#include <ysSpriteRenderer.h>
#include <ysShader.h>
#include <ysResources.h>
#include <YSapplication.h>
#include <ysRenderer.h>
#include "CameraScript.h"
#include "ysImgui_Manager.h"

#include <iostream>

extern ys::Application app;

ys::PlanetaryScene::PlanetaryScene()
{
}

ys::PlanetaryScene::~PlanetaryScene()
{
}

void ys::PlanetaryScene::Init()
{
	Scene::Init();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float factor = app.getScreenf().x;
	glGenBuffers(1, &axisVBO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glm::vec3 axis[] = {
		{-factor, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{factor, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{0.0f, -factor, 0.0f}, {0.0f, 1.0f, 0.0f},
		{0.0f, factor, 0.0f}, {0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, -factor}, {0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, factor}, {0.0f, 0.0f, 1.0f},
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);
	auto camera = object::Instantiate<GameObject>(enums::LayerType::Camera, glm::vec3(3.0f, 1.0f, 5.0f));
	renderer::mainCamera = camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();


	SetUpFBO(iToolSize_X, iToolSize_Y);

}

void ys::PlanetaryScene::Update()
{
	Scene::Update();
}

void ys::PlanetaryScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ys::PlanetaryScene::Render(HDC hDC, const int& index)
{
	RenderTo_Imgui_FBO(hDC, index); // imgui�� �׸���
	RenderTo_Imgui_FBO_Two(hDC, index); // imgui�� �׸���

	ys::Imgui_Manager::Get_Imgui_Manager()->SetFBO(Imgui_fbo, Imgui_fboTexture);
	ys::Imgui_Manager::Get_Imgui_Manager()->SetFBO_Two(Imgui_fbo_Two, Imgui_fboTexture_Two);

	// �̰� �Ϲ� ����ȭ��
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // imgui ������ ���۷� �ٲٰ�
	glViewport(0, 0, iImguiView_X, iImguiView_Y);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // �Ķ������� Ŭ���� ���� (RGBA: 0, 0, 1, 1)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// imgui�� �׸��͵�
	Scene::Render(hDC, -1);

	Scene::Render(hDC, index);
	auto shader = ys::Resources::Find<graphics::Shader>(L"vc");
	shader->Bind();

	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- ���� ��ȯ �� ����
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- ���� ��ȯ ����
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- ���� ���� ����: fovy, aspect, near, far
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 view = renderer::mainCamera->GetmainViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 axisWorldTrans{ 1.0f };
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(axisWorldTrans));

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_LINES, 0, 12 * 3);

}

void ys::PlanetaryScene::Destroy()
{
	Scene::Destroy();
}

void ys::PlanetaryScene::OnEnter()
{
}

void ys::PlanetaryScene::OnExit()
{
}

void ys::PlanetaryScene::SetUpFBO(int iX, int iY)
{
	glGenFramebuffers(1, &Imgui_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, Imgui_fbo);

	glGenTextures(1, &Imgui_fboTexture);
	glBindTexture(GL_TEXTURE_2D, Imgui_fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Imgui_fboTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// �ι�°
	glGenFramebuffers(1, &Imgui_fbo_Two);
	glBindFramebuffer(GL_FRAMEBUFFER, Imgui_fbo_Two);

	glGenTextures(1, &Imgui_fboTexture_Two);
	glBindTexture(GL_TEXTURE_2D, Imgui_fboTexture_Two);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Imgui_fboTexture_Two, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ys::PlanetaryScene::RenderTo_Imgui_FBO(HDC hDC, const int& index)
{

	glBindFramebuffer(GL_FRAMEBUFFER, Imgui_fbo); // imgui ������ ���۷� �ٲٰ�
	glViewport(0, 0, iImguiView_X, iImguiView_Y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �Ķ������� Ŭ���� ���� (RGBA: 0, 0, 1, 1)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// imgui�� �׸��͵�
	Scene::Render(hDC, -1);

	Scene::Render(hDC, index);
	auto shader = ys::Resources::Find<graphics::Shader>(L"vc");
	shader->Bind();

	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- ���� ��ȯ �� ����
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- ���� ��ȯ ����
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- ���� ���� ����: fovy, aspect, near, far
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 view = renderer::mainCamera->GetmainViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 axisWorldTrans{ 1.0f };
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(axisWorldTrans));

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_LINES, 0, 12 * 3);


	glBindFramebuffer(GL_FRAMEBUFFER, 0); // ���� ����ϴ� ������ ���۷� �ٲٱ�



}

void ys::PlanetaryScene::RenderTo_Imgui_FBO_Two(HDC hDC, const int& index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Imgui_fbo_Two); // imgui ������ ���۷� �ٲٰ�
	glViewport(0, 0, iImguiView_X, iImguiView_Y);
	glClearColor(0.2f, 0.6f, 0.3f, 1.0f); // �Ķ������� Ŭ���� ���� (RGBA: 0, 0, 1, 1)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// imgui�� �׸��͵�
	Scene::Render(hDC, -1);

	Scene::Render(hDC, index);
	auto shader = ys::Resources::Find<graphics::Shader>(L"vc");
	shader->Bind();

	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- ���� ��ȯ �� ����
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- ���� ��ȯ ����
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- ���� ���� ����: fovy, aspect, near, far
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 view = renderer::mainCamera->GetmainViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 axisWorldTrans{ 1.0f };
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(axisWorldTrans));

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_LINES, 0, 12 * 3);


	glBindFramebuffer(GL_FRAMEBUFFER, 0); // ���� ����ϴ� ������ ���۷� �ٲٱ�
}
