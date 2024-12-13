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

	glGenBuffers(1, &ssboSphere);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboSphere);


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

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	graphics::Vertex vertices[] = {
		{{-1.0f, -1.0f, 0.0f},		{0.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-left corner
		{{ 1.0f, -1.0f, 0.0f},		{1.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-right corner
		{{ 1.0f,  1.0f, 0.0f},		{1.0f, 1.0f},		{0.0f, 0.0f, 1.0f}},  // top-right corner
		{{-1.0f, -1.0f, 0.0f},		{0.0f, 0.0f},		{0.0f, 0.0f, 1.0f}},  // bottom-left corner
		{{ 1.0f,  1.0f, 0.0f},		{1.0f, 1.0f},		{0.0f, 0.0f, 1.0f}},  // top-right corner 
		{{-1.0f,  1.0f, 0.0f},		{0.0f, 1.0f},		{0.0f, 0.0f, 1.0f}}   // top-left corner
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	auto camera = object::Instantiate<GameObject>(enums::LayerType::Camera, glm::vec3(3.0f, 1.0f, 5.0f));
	renderer::mainCamera = camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();

	auto light = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(2.0f, 0.0f, 0.0f));
	auto lightSp = light->AddComponent<SpriteRenderer>();
	lightSp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
	lightSp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
	lightSp->SetObjectColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSp->AddLight(light, glm::vec3(1.0f, 1.0f, 1.0f));

	auto mainObject = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(0.0f, 0.0f, 2.0f));
	auto tr = mainObject->GetComponent<Transform>();
	//tr->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
	auto sp = mainObject->AddComponent<SpriteRenderer>();
	sp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
	sp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
	sp->SetObjectColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	sp->AddLight(light, glm::vec3(1.0f, 1.0f, 1.0f));
	SetUpFBO(iToolSize_X, iToolSize_Y);

	// 모든 객체 넣기	
	AllObject.push_back(light);
	AllObject.push_back(mainObject);
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

void ys::PlanetaryScene::PhongRender(HDC hDC, const int& index)
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, phongFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, phongTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Scene::Render(hDC, -1);

	auto shader = ys::Resources::Find<graphics::Shader>(L"vc");
	shader->Bind();

	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- 투영 변환 값 설정
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- 뷰잉 변환 설정
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	//Imgui_Manager::Get_Imgui_Manager()->SetObject_Matrix(mainObject->GetComponent<Transform>()->GetWorldMatrix()); //imgui에 객체의 월드행렬을 넘겨 줌
	
	
	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- 투영 공간 설정: fovy, aspect, near, far
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	

	glm::mat4 view = renderer::mainCamera->GetmainViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	//Imgui_Manager::Get_Imgui_Manager()->Test_Object(light);
	
	if (!Imgui_Manager::Get_Imgui_Manager()->isGizmoUsing())
	{
		float Closest_Dis = std::numeric_limits<float>::infinity();
		int Choice_Index = -1;

		float check_dis = 0.f;

		for (int i = 0; i < AllObject.size(); ++i)
		{
			check_dis = Imgui_Manager::Get_Imgui_Manager()->Check_Object(AllObject[i]);

			if (Closest_Dis > check_dis)
			{
				Closest_Dis = check_dis;
				Choice_Index = i;
			}

		}

		if (Choice_Index == -1)
		{
			Imgui_Manager::Get_Imgui_Manager()->SetObject(nullptr);
		}
		else
		{
			Imgui_Manager::Get_Imgui_Manager()->SetObject(AllObject[Choice_Index]);
		}
	}
	

	//Imgui_Manager::Get_Imgui_Manager()->SetObject(light);


	glm::mat4 axisWorldTrans{ 1.0f };
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(axisWorldTrans));

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_LINES, 0, 12 * 3);
	shader->Unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ys::PlanetaryScene::Render(HDC hDC, const int& index)
{
	if (index) return;
	// 이건 일반 메인화면 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, iImguiView_X, iImguiView_Y);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phong Render
	PhongRender(hDC, index);

	auto shader = ys::Resources::Find<graphics::Shader>(L"test");
	shader->Bind();
	glBindVertexArray(VAO);
	unsigned int projectionMatrix = glGetUniformLocation(shader->GetShaderID(), "projectionMatrix"); //--- 투영 변환 값 설정
	unsigned int viewMatrix = glGetUniformLocation(shader->GetShaderID(), "viewMatrix"); //--- 뷰잉 변환 설정
	unsigned int viewPosition = glGetUniformLocation(shader->GetShaderID(), "viewPosition");
	unsigned int viewportSize = glGetUniformLocation(shader->GetShaderID(), "viewportSize");

	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE
		, glm::value_ptr(renderer::mainCamera->GetmainProjectionMatrix()));

	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE
		, glm::value_ptr(renderer::mainCamera->GetmainViewMatrix()));

	auto pos = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();
	glUniform2fv(viewPosition, 1
		, glm::value_ptr(pos));
	std::cout << "viewPosition: " << pos.x << ' ' << pos.y << ' ' << pos.z << std::endl;
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
	shader->Unbind();

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
	glBindFramebuffer(GL_FRAMEBUFFER, phongFramebuffer);
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, iX, iY);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glGenTextures(1, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, currentTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &previousTexture);
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &phongTexture);
	glBindTexture(GL_TEXTURE_2D, phongTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}