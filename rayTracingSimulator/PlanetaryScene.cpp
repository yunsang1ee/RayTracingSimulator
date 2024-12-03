#include "PlanetaryScene.h"
#include <ysObject.h>
#include <ysSpriteRenderer.h>
#include <ysShader.h>
#include <ysResources.h>
#include <YSapplication.h>
#include <ysRenderer.h>
#include "CameraScript.h"

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
	Scene::Render(hDC, -1);

	Scene::Render(hDC, index);
	auto shader = ys::Resources::Find<graphics::Shader>(L"vc");
	shader->Bind();

	unsigned int projectionLocation = glGetUniformLocation(shader->GetShaderID(), "projectionTrans"); //--- 투영 변환 값 설정
	unsigned int viewLocation = glGetUniformLocation(shader->GetShaderID(), "viewTrans"); //--- 뷰잉 변환 설정
	unsigned int transformLocation = glGetUniformLocation(shader->GetShaderID(), "worldTrans");

	glm::mat4 projection = renderer::mainCamera->GetmainProjectionMatrix(); //--- 투영 공간 설정: fovy, aspect, near, far
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
