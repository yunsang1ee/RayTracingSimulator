#include "ysImgui_Manager.h"
#include <iostream>

#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include "ImGuizmo.h"
#include "ysGameObject.h"
#include "ysTransform.h"
#include "ysInputManager.h"
#include "ysRenderer.h"
#include "ysObject.h"
#include "ysSpriteRenderer.h"
#include "ysResources.h"


ys::Imgui_Manager* ys::Imgui_Manager::imgui_Manager = nullptr;

GLuint ys::Imgui_Manager::sourceFramebuffer = 0;
GLuint ys::Imgui_Manager::targetFramebuffer = 0;
GLuint ys::Imgui_Manager::phongTexture = 0;
GLuint ys::Imgui_Manager::raytracingTexture = 0;
GLuint ys::Imgui_Manager::resizeTexture = 0;


bool ys::Imgui_Manager::isPhongScreenHovered = false;
bool ys::Imgui_Manager::isObjectScreenHovered = false;


UINT ys::Imgui_Manager::rayPerPixel = 1;
UINT ys::Imgui_Manager::dispatch = 8;
bool ys::Imgui_Manager::isInvironment = false;
UINT ys::Imgui_Manager::maxBounceCount = 1;
UINT ys::Imgui_Manager::delay = 1;

float ys::Imgui_Manager::fov = 30.0f;


glm::vec3 ys::Imgui_Manager::CameraPos = glm::vec3(0,0,0);

int ys::Imgui_Manager::iPhongView_X = 640;
int ys::Imgui_Manager::iPhongView_Y = 360;


int ys::Imgui_Manager::iGizmo_type = ImGuizmo::OPERATION::TRANSLATE;

ys::GameObject* ys::Imgui_Manager::Object_Pointer = nullptr;



struct Ray
{
	glm::vec3 origin;
	glm::vec3 dir;
};


ys::Imgui_Manager::Imgui_Manager()
{
}

ys::Imgui_Manager::~Imgui_Manager()
{
}

void ys::Imgui_Manager::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}
void ys::Imgui_Manager::PostInit()
{
	glGenFramebuffers(1, &sourceFramebuffer);
	glGenFramebuffers(1, &targetFramebuffer);

	glGenTextures(1, &resizeTexture);
	glBindTexture(GL_TEXTURE_2D, resizeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 360, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ys::Imgui_Manager::Update()
{
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();

	//ImGui::Begin("Viewport");
	//ImTextureID texID = (ImTextureID)(intptr_t)fboTexture;
	//ImGui::Image(texID, ImVec2(800, 600));
	//ImGui::End(); // 이거 꼭 해주자

}

void ys::Imgui_Manager::LateUpdate()
{


}

void ys::Imgui_Manager::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();


	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // 고정된 위치 
	ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // 고정된 크기
	//ImGui::SetNextWindowSize(ImVec2(640 * 2, 375 * 2), ImGuiCond_Always); // 고정된 크기

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Phong", nullptr, window_flags);

	isPhongScreenHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

	ImGui::Image(phongTexture, ImVec2(iPhongView_X, iPhongView_Y), ImVec2(0, 1), ImVec2(1, 0));

	Change_Transform_Object();

	
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(0, 390), ImGuiCond_Always); // 고정된 위치 
	ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // 고정된 크기

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ImGui::Begin("Pathtracing", nullptr, window_flags);
	ImGui::Image(raytracingTexture, ImVec2(640, 360), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(1920, 0), ImGuiCond_Always); // 한번만 위치 설정(이동 가능)
	ImGui::SetNextWindowSize(ImVec2(400, 1080), ImGuiCond_Always); // 한번만 창 크기 설정(크기 조정 가능)
	ImGuiWindowFlags tool_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Object", nullptr, tool_flags);


	// 선택할 변수 

	static bool invironment = isInvironment; // 라디오 버튼 추가 
	ImGui::Checkbox("isInvironment", &invironment);
	isInvironment = invironment;

	ImGui::Text("");
	ImGui::Text("Dispath");
	static int dispath = dispatch; // 라디오 버튼 추가 
	
	ImGui::RadioButton("fargment", &dispath, 0);
	ImGui::RadioButton("dispath 8", &dispath, 8);
	ImGui::RadioButton("dispath 16", &dispath, 16);
	ImGui::RadioButton("dispath 32", &dispath, 32); // 현재 선택한 옵션 출력 
	dispatch = dispath;
	ImGui::Text("Selected Option: %d", dispath);


	
	isObjectScreenHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

	int Ray_Per_Pixel = rayPerPixel;
	ImGui::Text("");
	ImGui::Text("Ray Per Pixel");
	ImGui::SliderInt("Ray Per Pixel", &Ray_Per_Pixel, 1, 100, "Value = %d"); // 현재 변수 값 출력
	rayPerPixel = Ray_Per_Pixel;
	ImGui::Text("Ray Per Pixel: %d", rayPerPixel);


	int MaxBounceCount = maxBounceCount;
	ImGui::Text("");
	ImGui::Text("Max Bounce Count");
	ImGui::SliderInt("Max Bounce Count", &MaxBounceCount, 1, 100, "Value = %d"); // 현재 변수 값 출력
	maxBounceCount = MaxBounceCount;
	ImGui::Text("Max Bounce Count: %d", maxBounceCount);

	int delay = Imgui_Manager::delay;
	ImGui::Text("");
	ImGui::Text("Frame Delay");
	ImGui::SliderInt("Frame Delay", &delay, 1, 100, "Value = %d"); // 현재 변수 값 출력
	Imgui_Manager::delay = delay;
	ImGui::Text("Frame Delay: %d", delay);

	ImGui::Text("");
	ImGui::Text("Fov");
	ImGui::SliderFloat("Fov", &fov, 10, 90, "Value = %.3f"); // 현재 변수 값 출력
	ImGui::Text("Fov: %.3f", fov);

	if (Object_Pointer != nullptr)
	{
		// 색상 변수 선언
		glm::vec4 color1 = { Object_Pointer->GetComponent<SpriteRenderer>()->GetMaterial().color}; // 빨강 
		// 색상 편집기 추가 
		float Color[3] = { color1.x,color1.y,color1.z };

		ImGui::Text("");
		ImGui::Text("Picking_Object_Color");
		ImGui::ColorEdit3("Picking_Object_Color", Color);
		// 색상 버튼 추가 (색상 미리보기) 
		Object_Pointer->GetComponent<SpriteRenderer>()->SetObjectColor(glm::vec4(Color[0], Color[1], Color[2], color1.w));

		ImGui::ColorButton("Object_Color", ImVec4(color1.x, color1.y, color1.z,1.0));

		float smoothness = Object_Pointer->GetComponent<SpriteRenderer>()->GetMaterial().smoothness;
		ImGui::Text("");
		ImGui::Text("Smoothness");
		ImGui::SliderFloat("Smoothness", &smoothness, 0.f, 1.f, "Value = %.3f"); // 현재 변수 값 출력
		Object_Pointer->GetComponent<SpriteRenderer>()->SetSmoothness(smoothness);
		ImGui::Text("Smoothness: %.3f", smoothness);

		
		float specularProbability = Object_Pointer->GetComponent<SpriteRenderer>()->GetMaterial().specularProbability;
		ImGui::Text("");
		ImGui::Text("SpecularProbability");
		ImGui::SliderFloat("SpecularProbability", &specularProbability, 0.f, 1.f, "Value = %.3f"); // 현재 변수 값 출력
		Object_Pointer->GetComponent<SpriteRenderer>()->SetSpecularProbability(specularProbability);
		ImGui::Text("SpecularProbability: %.3f", specularProbability);

		
		// 색상 변수 선언
		glm::vec4 EmittedColor = { Object_Pointer->GetComponent<SpriteRenderer>()->GetMaterial().emittedColor }; // 빨강 
		// 색상 편집기 추가 
		float Emitted_Color[3] = { EmittedColor.x,EmittedColor.y,EmittedColor.z };
		ImGui::Text("");
		ImGui::Text("EmittedColor");
		ImGui::ColorEdit3("EmittedColor", Emitted_Color);
		// 색상 버튼 추가 (색상 미리보기) 
		Object_Pointer->GetComponent<SpriteRenderer>()->SetLightColor(glm::vec4(Emitted_Color[0], Emitted_Color[1], Emitted_Color[2], EmittedColor.w));

		ImGui::ColorButton("EmittedColor", ImVec4(EmittedColor.x, EmittedColor.y, EmittedColor.z, 1.0));


		float emissionstrength = Object_Pointer->GetComponent<SpriteRenderer>()->GetMaterial().emissionStrength;
		ImGui::Text("");
		ImGui::Text("emissionStrength");
		ImGui::SliderFloat("emissionStrength", &emissionstrength, 0.f, 10.f, "Value = %.3f"); // 현재 변수 값 출력
		Object_Pointer->GetComponent<SpriteRenderer>()->SetLightStrength(emissionstrength);
		ImGui::Text("emissionStrength: %.3f", emissionstrength);
				

	}
	else
	{
		//static float color1[3] = { 0.0f, 0.0f, 0.0f }; // 검정
		//ImGui::ColorEdit3("Picking_Object_Color", color1);
		//ImGui::ColorButton("Object_Color", ImVec4(color1[0], color1[1], color1[2], 1.0f));
	}

	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ys::Imgui_Manager::Release()
{

}

void ys::Imgui_Manager::Destroy()
{

	imgui_Manager->Release(); // 변수 해제


	// imgui 닫기
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete imgui_Manager;
	imgui_Manager = nullptr;

}

void ys::Imgui_Manager::SetFBO(GLuint fboTexture)
{
	phongTexture = fboTexture;
}

void ys::Imgui_Manager::SetFBO_Two(GLuint fboTexture)
{
	raytracingTexture = fboTexture;
}


void ys::Imgui_Manager::SetObject(ys::GameObject* Game_Object)
{
	Object_Pointer = Game_Object;
}

float ys::Imgui_Manager::RaySphere(Ray ray, glm::vec3 sphereCenter, float sphereRadius)
{
	glm::vec3 offsetRayOrigin = ray.origin - sphereCenter;

	float a = glm::dot(ray.dir, ray.dir);
	float b = 2.0f * glm::dot(offsetRayOrigin, ray.dir);
	float c = glm::dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0) {
		float sqrtDiscriminant = sqrt(discriminant);
		float t1 = (-b - sqrtDiscriminant) / (2.0f * a);

		if (t1 >= 0) {

			float dst = (-b - sqrt(sqrtDiscriminant)) / (2 * a);

			return dst;
		}
	}

	return std::numeric_limits<float>::infinity();
}

glm::vec3 ys::Imgui_Manager::GenRayDir()
{
	// 마우스 위치 받아오기
	ImGuiIO& io = ImGui::GetIO();
	glm::vec2 mousePos = glm::vec2(io.MousePos.x * 1920 / iPhongView_X, io.MousePos.y * 1080 / iPhongView_Y); // 마우스 위치를 1920,1080으로 변환

	glm::vec4 clipSpace = glm::vec4((mousePos / glm::vec2(1920, 1080) * 2.0f - 1.0f), 1.0f, 1.0f); // ?
	clipSpace.y *= -1;

	glm::vec4 viewSpace = inverse(renderer::mainCamera->GetmainProjectionMatrix()) * clipSpace;
	viewSpace.w = 1.0f;

	glm::vec3 worldSpace = (inverse(renderer::mainCamera->GetmainViewMatrix()) * viewSpace);

	return normalize(worldSpace - renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition());
}

float ys::Imgui_Manager::Check_Object(glm::vec3 rayDir, ys::GameObject* Game_Object)
{

	// 이 함수는 마우스를 눌렀을 때만 들어와야 함
	float sphereRadius1 = Game_Object->GetComponent<Transform>()->GetScale().x;

	// 구체 교차 검사
	Ray ray;
	ray.origin = renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();
	ray.dir = rayDir;

	return RaySphere(ray, glm::vec3(Game_Object->GetComponent<Transform>()->GetPosition()), sphereRadius1);
}

void ys::Imgui_Manager::Test_Object(ys::GameObject* Game_Object)
{
	Object_Pointer = Game_Object;

}



bool ys::Imgui_Manager::isGizmoUsing()
{
	if ((ImGuizmo::IsUsing() || !(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isPhongScreenHovered)))
	{
		return true;
	}

	return false;

}

bool ys::Imgui_Manager::isObjectChange()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isObjectScreenHovered)
	{
		return true;
	}

	return false;

}

void ys::Imgui_Manager::Change_Transform_Object()
{
	if (ys::InputManager::getKeyDown('q'))
	{
		iGizmo_type = ImGuizmo::OPERATION::TRANSLATE;
	}

	if (ys::InputManager::getKeyDown('e'))
	{
		iGizmo_type = ImGuizmo::OPERATION::SCALE;
	}

	if (ys::InputManager::getKeyDown('r'))
	{
		iGizmo_type = ImGuizmo::OPERATION::ROTATE;
	}

	

	if (Object_Pointer != nullptr && iGizmo_type != -1) // 물체를 받아와서 물체가 있다면? 검사
	{
		// gizmos
		ImGuizmo::SetOrthographic(false); // 원근 투영에 하고 싶으니까 직교 끄기
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();


		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight);

		// 카메라를 가져와야 함

		glm::mat4 Object_Matrix = Object_Pointer->GetComponent<Transform>()->GetWorldMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(renderer::mainCamera->GetmainViewMatrix()), glm::value_ptr(renderer::mainCamera->GetmainProjectionMatrix()),
			(ImGuizmo::OPERATION)iGizmo_type, ImGuizmo::LOCAL, glm::value_ptr(Object_Matrix));

		if (ImGuizmo::IsUsing())
		{
			float translation[3], rotation[3], scale[3];

			float ObjMatrix[4][4] = {};

			memcpy(ObjMatrix, &Object_Matrix, 4 * 4 * sizeof(float));

			ImGuizmo::DecomposeMatrixToComponents(&ObjMatrix[0][0], translation, rotation, scale);


			// 회전
			glm::vec3 Origin_Rotation = Object_Pointer->GetComponent<Transform>()->GetRotation(); // 현재 원래 회전 값
			glm::vec3 Delta_Rotation = glm::vec3(rotation[0], rotation[1], -rotation[2]) - Origin_Rotation; // 회전 변화량 // z 축 반전을 하지않으면 회전이 이상하게 됨
			if (Delta_Rotation[0] != 0.0f)
			{
				int i = 5;
			}

			Origin_Rotation += Delta_Rotation; // 변화량 더하기

			Object_Pointer->GetComponent<Transform>()->SetPosition(glm::vec3(translation[0], translation[1], translation[2]));
			Object_Pointer->GetComponent<Transform>()->SetRotation(Origin_Rotation);
			Object_Pointer->GetComponent<Transform>()->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
		}
	}

}
