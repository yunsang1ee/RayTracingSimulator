#include "ysImgui_Manager.h"
#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
ys::Imgui_Manager* ys::Imgui_Manager::imgui_Manager = nullptr;

GLuint ys::Imgui_Manager::fbo = 0;
GLuint ys::Imgui_Manager::fboTexture = 0;

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

	ImGui::Begin("Viewport");
	ImTextureID texID = (ImTextureID)(intptr_t)fboTexture;
	ImGui::Image(texID, ImVec2(800, 600));
	ImGui::End(); // 이거 꼭 해주자

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

void ys::Imgui_Manager::GetFBO(GLuint _fbo, GLuint _fboTexture)
{
	fbo = _fbo;
		
	fboTexture = _fboTexture;
}


