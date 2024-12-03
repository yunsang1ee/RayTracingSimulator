#include "ysImgui_Manager.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
ys::Imgui_Manager* ys::Imgui_Manager::imgui_Manager = nullptr;

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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ViewPort");

	ImGui::End(); // 이거 꼭 해주자
}

void ys::Imgui_Manager::LateUpdate()
{


}

void ys::Imgui_Manager::Render()
{
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
