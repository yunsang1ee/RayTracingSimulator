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
	: ssboSphereSize(sizeof(Sphere) * 10)
	, screenSize(1920, 1080)
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

	GenObject();

	glGenBuffers(1, &ssboSphere);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboSphere);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, ssboSphereSize, nullptr
		, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	spheresPointer = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssboSphereSize
		, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboSphere);

	// sky_box_texture
	//Imgui_Manager::Get_Imgui_Manager()->Push_Sky_Box_Texture(Resources::Find<graphics::Texture>(L"Skybox_star"));

	SetUpFBO(screenSize.x, screenSize.y);
}

void ys::PlanetaryScene::Update()
{
	Scene::Update();

	if (InputManager::getKeyDown('o')) Imgui_Manager::Get_Imgui_Manager()->SetObject(nullptr);
	if (InputManager::getKeyDown('m')) isMotionBlur = !isMotionBlur;

	if (InputManager::getKeyDown(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(app.getWindow(), GL_TRUE);
}

void ys::PlanetaryScene::LateUpdate()
{
	auto prevCameraMatrix = renderer::mainCamera->GetmainViewMatrix();
	Scene::LateUpdate();
	auto cameraMatrix = renderer::mainCamera->GetmainViewMatrix();
	if (prevCameraMatrix != cameraMatrix && !isMotionBlur)
		frameCount = 0;
}

void ys::PlanetaryScene::PhongRender(HDC hDC, const int& index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, phongFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, phongTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	if (ys::InputManager::getKeyDown('b'))
	{
		if (Imgui_Manager::Get_Imgui_Manager()->GetPickedObject() != nullptr)
			Imgui_Manager::Get_Imgui_Manager()->GetPickedObject()
			->GetComponent<SpriteRenderer>()->GetMesh()->GetBVH().DrawNodes();
	}


	Scene::Render(hDC, -1);

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
	shader->Unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ys::PlanetaryScene::Render(HDC hDC, const int& index)
{
	if (index != 0)return;

	// Gizmo Update
	if (!Imgui_Manager::Get_Imgui_Manager()->isGizmoUsing())
	{
		float Closest_Dis = std::numeric_limits<float>::infinity();
		int Choice_Index = -1;

		float check_dis = 0.f;

		auto rayDir = Imgui_Manager::Get_Imgui_Manager()->GenRayDir();
		auto AllObject = GetLayer(enums::LayerType::Object)->GetGameObjects();

		for (int i = 0; i < AllObject.size(); ++i)
		{
			check_dis = Imgui_Manager::Get_Imgui_Manager()->Check_Object(rayDir, AllObject[i]);

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
	else
	{
		auto object = Imgui_Manager::Get_Imgui_Manager()->GetPickedObject();
		if (object)
		{
			spheresIndex[reinterpret_cast<uintptr_t>(object)].first = true;
			frameCount = 0;
		}
	}

	if (Imgui_Manager::Get_Imgui_Manager()->isObjectChange()) frameCount = 0;
	// SSBO Update
	UpdateSSBO();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboSphere);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboSphere);

	// Phong Render
	if (frameCount == 0) PhongRender(hDC, index);

	// RayTracing
	auto dispatch = Imgui_Manager::Get_Imgui_Manager()->Get_Dispatch();
	if (!dispatch)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, phongFramebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		glViewport(0, 0, screenSize.x, screenSize.y);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboSphere);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboSphere);
		auto shader = ys::Resources::Find<graphics::Shader>(L"test");
		shader->Bind();
		glBindVertexArray(VAO);
		{
			unsigned int invProjectionMatrixLoc = glGetUniformLocation(shader->GetShaderID(), "invProjectionMatrix");
			unsigned int invViewMatrixLoc = glGetUniformLocation(shader->GetShaderID(), "invViewMatrix");
			unsigned int viewPositionLoc = glGetUniformLocation(shader->GetShaderID(), "viewPosition");
			unsigned int viewportSizeLoc = glGetUniformLocation(shader->GetShaderID(), "viewportSize");
			unsigned int maxBounceCountLoc = glGetUniformLocation(shader->GetShaderID(), "maxBounceCount");
			unsigned int screenSizeLoc = glGetUniformLocation(shader->GetShaderID(), "screenSize");
			unsigned int numRenderedFrameLoc = glGetUniformLocation(shader->GetShaderID(), "numRenderedFrame");
			unsigned int rayPerPixelLoc = glGetUniformLocation(shader->GetShaderID(), "rayPerPixel");

			glUniformMatrix4fv(invProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(renderer::mainCamera->GetmainProjectionMatrix())));
			glUniformMatrix4fv(invViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(renderer::mainCamera->GetmainViewMatrix())));
			glUniform3fv(viewPositionLoc, 1, glm::value_ptr(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));
			glUniform2fv(viewportSizeLoc, 1, glm::value_ptr(glm::vec2(this->screenSize)));
			glUniform1ui(maxBounceCountLoc, Imgui_Manager::Get_Imgui_Manager()->Get_MaxBounceCount());
			glUniform2uiv(screenSizeLoc, 1, glm::value_ptr(screenSize));
			glUniform1ui(numRenderedFrameLoc, frameCount);
			glUniform1ui(rayPerPixelLoc, Imgui_Manager::Get_Imgui_Manager()->Get_RayPerPixel());

			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, position));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, texture));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, normal));
			glEnableVertexAttribArray(2);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
		shader->Unbind();
	}
	else
	{
		auto shader = Resources::Find<graphics::Shader>(L"raytracing" + std::to_wstring(Imgui_Manager::Get_Imgui_Manager()->Get_Dispatch()));
		shader->Bind();
		glBindImageTexture(1, currentTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
		glClearTexImage(currentTexture, 0, GL_RGBA, GL_UNSIGNED_INT, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

		unsigned int invProjectionMatrixLoc = glGetUniformLocation(shader->GetShaderID(), "invProjectionMatrix");
		unsigned int invViewMatrixLoc = glGetUniformLocation(shader->GetShaderID(), "invViewMatrix");
		unsigned int viewPositionLoc = glGetUniformLocation(shader->GetShaderID(), "viewPosition");
		unsigned int viewportSizeLoc = glGetUniformLocation(shader->GetShaderID(), "viewportSize");
		unsigned int maxBounceCountLoc = glGetUniformLocation(shader->GetShaderID(), "maxBounceCount");
		unsigned int screenSizeLoc = glGetUniformLocation(shader->GetShaderID(), "screenSize");
		unsigned int numRenderedFrameLoc = glGetUniformLocation(shader->GetShaderID(), "numRenderedFrame");
		unsigned int rayPerPixelLoc = glGetUniformLocation(shader->GetShaderID(), "rayPerPixel");

		glUniformMatrix4fv(invProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(renderer::mainCamera->GetmainProjectionMatrix())));
		glUniformMatrix4fv(invViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(renderer::mainCamera->GetmainViewMatrix())));
		glUniform3fv(viewPositionLoc, 1, glm::value_ptr(renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition()));
		glUniform2fv(viewportSizeLoc, 1, glm::value_ptr(glm::vec2(this->screenSize)));
		glUniform1ui(maxBounceCountLoc, Imgui_Manager::Get_Imgui_Manager()->Get_MaxBounceCount());
		glUniform2uiv(screenSizeLoc, 1, glm::value_ptr(screenSize));
		glUniform1ui(numRenderedFrameLoc, frameCount);
		glUniform1ui(rayPerPixelLoc, Imgui_Manager::Get_Imgui_Manager()->Get_RayPerPixel());

		if (dispatch == 8)
			shader->DispatchCompute(glm::vec2(1920, 1080), graphics::Shader::DispatchMode::dispatch8x8x1);
		else if (dispatch == 16)
			shader->DispatchCompute(glm::vec2(1920, 1080), graphics::Shader::DispatchMode::dispatch16x16x1);
		else if (dispatch == 32)
			shader->DispatchCompute(glm::vec2(1920, 1080), graphics::Shader::DispatchMode::dispatch32x32x1);

		glBindFramebuffer(GL_FRAMEBUFFER, phongFramebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		glViewport(0, 0, screenSize.x, screenSize.y);

		shader = Resources::Find<graphics::Shader>(L"renderTexture");
		shader->Bind();
		GLint textureLocation = glGetUniformLocation(shader->GetShaderID(), "textureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		glUniform1i(textureLocation, 0);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, texture));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (void*)offsetof(graphics::Vertex, normal));
		glEnableVertexAttribArray(2);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	if (frameCount == 0)
		glCopyImageSubData(phongTexture, GL_TEXTURE_2D, 0, 0, 0, 0, 
			previousTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
			1920, 1080, 1);

	// raytracing mix
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto mixShader = Resources::Find<graphics::Shader>(L"mix");
	mixShader->Bind();

	GLint oldTexture = glGetUniformLocation(mixShader->GetShaderID(), "oldTexture");
	GLint newTexture = glGetUniformLocation(mixShader->GetShaderID(), "newTexture");
	GLint mixNumRenderedFrame = glGetUniformLocation(mixShader->GetShaderID(), "numRenderedFrame");
	GLint delay = glGetUniformLocation(mixShader->GetShaderID(), "delay");
	GLint viewSize = glGetUniformLocation(mixShader->GetShaderID(), "viewSize");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, currentTexture);
	glUniform1i(newTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glUniform1i(oldTexture, 1);
	
	glUniform2uiv(viewSize, 1, glm::value_ptr(this->screenSize));

	//glUniform1ui(mixNumRenderedFrame, );
	glUniform1f(mixNumRenderedFrame, this->frameCount / (float)Imgui_Manager::Get_Imgui_Manager()->Get_Delay());


	glDrawArrays(GL_TRIANGLES, 0, 6);
	mixShader->Unbind();

	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, 1920, 1080, 0);


	frameCount++;
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &previousTexture);
	glBindTexture(GL_TEXTURE_2D, previousTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &phongTexture);
	glBindTexture(GL_TEXTURE_2D, phongTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iX, iY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ys::PlanetaryScene::GenObject()
{
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

	{
		auto light = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(-43.03f, 33.94f, 78.11f));
		auto lightTr = light->GetComponent<Transform>();
		auto lightSp = light->AddComponent<SpriteRenderer>();
		lightTr->SetScale(glm::vec3(24.67f, 24.67f, 24.67f));
		lightSp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
		lightSp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
		lightSp->SetObjectColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		lightSp->SetLightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		lightSp->SetLightStrength(1.0f);
		lightSp->IsSun();

		auto scale = lightTr->GetScale();

		spheresIndex.emplace(reinterpret_cast<uintptr_t>(light), std::make_pair(false, spheres.size()));
		spheres.emplace_back(Sphere{
				lightTr->GetPosition()
				, std::min({abs(scale.x),abs(scale.y),abs(scale.z)})
				, lightSp->GetMaterial()
			}
		);
	}
	{
		auto light = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(-2.26f, 0.78f, 2.05f));
		auto lightTr = light->GetComponent<Transform>();
		auto lightSp = light->AddComponent<SpriteRenderer>();
		lightSp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
		lightSp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
		lightSp->SetObjectColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		lightSp->SetSmoothness(1.0f);

		auto scale = lightTr->GetScale();

		spheresIndex.emplace(reinterpret_cast<uintptr_t>(light), std::make_pair(false, spheres.size()));
		spheres.emplace_back(Sphere{
				lightTr->GetPosition()
				, std::min({abs(scale.x),abs(scale.y),abs(scale.z)})
				, lightSp->GetMaterial()
			}
		);
	}
	{
		auto light = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(-4.26f, 0.78f, 2.05f));
		auto lightTr = light->GetComponent<Transform>();
		auto lightSp = light->AddComponent<SpriteRenderer>();
		lightSp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
		lightSp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
		lightSp->SetObjectColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		lightSp->SetSmoothness(1.0f);

		auto scale = lightTr->GetScale();

		spheresIndex.emplace(reinterpret_cast<uintptr_t>(light), std::make_pair(false, spheres.size()));
		spheres.emplace_back(Sphere{
				lightTr->GetPosition()
				, std::min({abs(scale.x),abs(scale.y),abs(scale.z)})
				, lightSp->GetMaterial()
			}
		);
	}
	{
		auto light = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(0.06f, 0.57f, 1.98f));
		auto lightTr = light->GetComponent<Transform>();
		auto lightSp = light->AddComponent<SpriteRenderer>();
		lightSp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
		lightSp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
		lightSp->SetObjectColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		lightSp->SetSmoothness(0.0f);

		auto scale = lightTr->GetScale();

		spheresIndex.emplace(reinterpret_cast<uintptr_t>(light), std::make_pair(false, spheres.size()));
		spheres.emplace_back(Sphere{
				lightTr->GetPosition()
				, std::min({abs(scale.x),abs(scale.y),abs(scale.z)})
				, lightSp->GetMaterial()
			}
		);
	}


	auto mainObject = object::Instantiate<GameObject>(enums::LayerType::Object, glm::vec3(0.0f, -10.51f, 2.0f));
	auto tr = mainObject->GetComponent<Transform>();
	tr->SetScale(glm::vec3(10.41f, 10.41f, 10.41f));
	auto sp = mainObject->AddComponent<SpriteRenderer>();
	sp->SetShader(Resources::Find<graphics::Shader>(L"phong"));
	sp->SetMesh(Resources::Find<Mesh>(L"Sphere"));
	sp->SetObjectColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	sp->SetSmoothness(0.0f);

		auto scale = tr->GetScale();
		spheresIndex.emplace(reinterpret_cast<uintptr_t>(mainObject), std::make_pair(false, spheres.size()));
		spheres.emplace_back(Sphere{
				tr->GetPosition()
				, std::min({abs(scale.x),abs(scale.y),abs(scale.z)})
				, sp->GetMaterial()
			}
		);
	}


void ys::PlanetaryScene::UpdateSSBO()
{
	for (auto& [key, value] : spheresIndex)
	{
		auto& [dirty, index] = value;
		if (!dirty) continue;

		auto object = reinterpret_cast<GameObject*>(key);
		auto tr = object->GetComponent<Transform>();
		auto sp = object->GetComponent<SpriteRenderer>();

		spheres[index].center = tr->GetPosition();

		auto scale = tr->GetScale();
		spheres[index].radius = std::min({ abs(scale.x),abs(scale.y),abs(scale.z) });
		spheres[index].material = sp->GetMaterial();
		dirty = false;
	}

	size_t dataSize = spheres.size() * sizeof(Sphere);

	if (dataSize > ssboSphereSize)
	{
		ssboSphereSize *= 2;

		GLuint newSSBO;
		glGenBuffers(1, &newSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, newSSBO);

		glBufferStorage(GL_SHADER_STORAGE_BUFFER, ssboSphereSize, nullptr
			, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

		spheresPointer = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssboSphereSize
			, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

		glDeleteBuffers(1, &ssboSphere);

		ssboSphere = newSSBO;
	}

	memcpy(spheresPointer, spheres.data(), dataSize);
}
