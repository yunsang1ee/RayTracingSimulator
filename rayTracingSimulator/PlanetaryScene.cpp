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
#include <ysInputManager.h>

extern ys::Application app;

ys::PlanetaryScene::PlanetaryScene()
	: iImguiView_X(1920), iImguiView_Y(1080), iToolSize_X(1920), iToolSize_Y(1080)
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
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	graphics::Vertex vertices[] = {
		{{-1.0f, -1.0f, 0.0f},		{0.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-left corner
		{{ 1.0f, -1.0f, 0.0f},		{1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-right corner
		{{ 1.0f,  1.0f, 0.0f},		{1.0f, 1.0f},		{0.0f, 0.0f, 1.0f}},  // top-right corner
		{{-1.0f, -1.0f, 0.0f},		{0.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-left corner (duplicate for triangle strip)
		{{ 1.0f,  1.0f, 0.0f},		{1.0f, 1.0f},		{0.0f, 0.0f, 1.0f}},  // top-right corner (duplicate for triangle strip)
		{{-1.0f,  1.0f, 0.0f},		{0.0f, 1.0f},		{0.0f, 0.0f, 1.0f}}   // top-left corner
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	auto camera = object::Instantiate<GameObject>(enums::LayerType::Camera, glm::vec3(0.0f, 0.0f, 0.0f));
	renderer::mainCamera = camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();


	SetUpFBO(iToolSize_X, iToolSize_Y);

}

void ys::PlanetaryScene::Update()
{
	Scene::Update();

	if (InputManager::getKeyDown(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(app.getWindow(), GL_TRUE);
}

void ys::PlanetaryScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ys::PlanetaryScene::Render(HDC hDC, const int& index)
{
	// 이건 일반 메인화면 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // imgui 프레임 버퍼로 바꾸고
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, phongTexture, 0);
	glViewport(0, 0, iImguiView_X, iImguiView_Y);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// imgui에 그릴것들
	Scene::Render(hDC, index);
	auto shader = ys::Resources::Find<graphics::Shader>(L"phong");
	shader->Bind();
	glBindVertexArray(VAO);

	unsigned int projectionMatrix = glGetUniformLocation(shader->GetShaderID(), "projectionMatrix"); //--- 투영 변환 값 설정
	unsigned int viewMatrix = glGetUniformLocation(shader->GetShaderID(), "viewMatrix"); //--- 뷰잉 변환 설정
	unsigned int _WorldSpaceCameraPos = glGetUniformLocation(shader->GetShaderID(), "_WorldSpaceCameraPos");
	unsigned int viewportSize = glGetUniformLocation(shader->GetShaderID(), "viewportSize");

	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE
		, glm::value_ptr(renderer::mainCamera->GetmainProjectionMatrix()));

	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE
		, glm::value_ptr(renderer::mainCamera->GetmainViewMatrix()));

	glUniform3fv(_WorldSpaceCameraPos, 1
		, glm::value_ptr(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));

	glUniform2fv(viewportSize, 1
		, glm::value_ptr(glm::vec2(iImguiView_X, iImguiView_Y)));

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, texture));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, normal));
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//prevTexture Update
	glCopyImageSubData(currentTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
		previousTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
		1920, 1080, 1);

	ys::Imgui_Manager::Get_Imgui_Manager()->SetFBO(phongTexture);
	ys::Imgui_Manager::Get_Imgui_Manager()->SetFBO_Two(currentTexture);
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
	glGenFramebuffers(1, &phongFramebuffer);
	glGenFramebuffers(1, &finalFramebuffer);

	glGenTextures(1, &currentTexture);
	glGenTextures(1, &previousTexture);
	glGenTextures(1, &phongTexture);
	glGenTextures(1, &finalTexture);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, phongTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}