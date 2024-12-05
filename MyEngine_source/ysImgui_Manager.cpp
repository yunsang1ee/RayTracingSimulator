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

GLuint ys::Imgui_Manager::fbo_Two = 0;
GLuint ys::Imgui_Manager::fboTexture_Two = 0;

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
	//ImGui::End(); // �̰� �� ������

}

void ys::Imgui_Manager::LateUpdate()
{


}

void ys::Imgui_Manager::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // ������ ��ġ 
	ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // ������ ũ��

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Viewport", nullptr, window_flags);
	ImTextureID texID = (ImTextureID)(intptr_t)fboTexture;
	ImGui::Image(texID, ImVec2(640, 360), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End(); // �̰� �� ������



	ImGui::SetNextWindowPos(ImVec2(0, 390), ImGuiCond_Always); // ������ ��ġ 
	ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // ������ ũ��

	ImGui::Begin("View", nullptr, window_flags);
	texID = (ImTextureID)(intptr_t)fboTexture_Two;
	ImGui::Image(texID, ImVec2(640, 360), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End(); // �̰� �� ������


	ImGui::SetNextWindowPos(ImVec2(1920, 0), ImGuiCond_Always); // �ѹ��� ��ġ ����(�̵� ����)
	ImGui::SetNextWindowSize(ImVec2(400, 1080), ImGuiCond_Always); // �ѹ��� â ũ�� ����(ũ�� ���� ����)
	ImGuiWindowFlags tool_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Object", nullptr, tool_flags);
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ys::Imgui_Manager::Release()
{

}

void ys::Imgui_Manager::Destroy()
{

	imgui_Manager->Release(); // ���� ����


	// imgui �ݱ�
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete imgui_Manager;
	imgui_Manager = nullptr;

}

void ys::Imgui_Manager::SetFBO(GLuint _fbo, GLuint _fboTexture)
{
	fbo = _fbo;
		
	fboTexture = _fboTexture;
}

void ys::Imgui_Manager::SetFBO_Two(GLuint _fbo_Two, GLuint _fboTexture_Two)
{
	fbo_Two = _fbo_Two;

	fboTexture_Two = _fboTexture_Two;
}


