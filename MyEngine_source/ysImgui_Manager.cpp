#include "ysImgui_Manager.h"

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


ys::Imgui_Manager* ys::Imgui_Manager::imgui_Manager = nullptr;

GLuint ys::Imgui_Manager::sourceFramebuffer = 0;
GLuint ys::Imgui_Manager::targetFramebuffer = 0;
GLuint ys::Imgui_Manager::phongTexture = 0;
GLuint ys::Imgui_Manager::raytracingTexture = 0;
GLuint ys::Imgui_Manager::resizeTexture = 0;


int ys::Imgui_Manager::iGizmo_type = ImGuizmo::OPERATION::TRANSLATE;



glm::mat4 ys::Imgui_Manager::CameraMatrix = glm::mat4(1.0);
glm::mat4 ys::Imgui_Manager::Projection = glm::mat4(1.0);
ys::GameObject* ys::Imgui_Manager::Object_Pointer = nullptr;



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
	ImGuizmo::BeginFrame();


	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // ������ ��ġ 
	//ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // ������ ũ��
	ImGui::SetNextWindowSize(ImVec2(640 * 2, 375 * 2), ImGuiCond_Always); // ������ ũ��

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Phong", nullptr, window_flags);

	ImGui::Image(phongTexture, ImVec2(640 * 2, 360 * 2), ImVec2(0, 1), ImVec2(1, 0));

	Change_Transform_Object();

	
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(0, 390), ImGuiCond_Always); // ������ ��ġ 
	ImGui::SetNextWindowSize(ImVec2(640, 390), ImGuiCond_Always); // ������ ũ��

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ImGui::Begin("Pathtracing", nullptr, window_flags);
	ImGui::Image(resizeTexture, ImVec2(640, 360), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(1920, 0), ImGuiCond_Always); // �ѹ��� ��ġ ����(�̵� ����)
	ImGui::SetNextWindowSize(ImVec2(400, 1080), ImGuiCond_Always); // �ѹ��� â ũ�� ����(ũ�� ���� ����)
	ImGuiWindowFlags tool_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Object", nullptr, tool_flags);



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

	imgui_Manager->Release(); // ���� ����


	// imgui �ݱ�
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

void ys::Imgui_Manager::SetCamera_Matrix(glm::mat4 _mat)
{
	CameraMatrix = _mat;

}

void ys::Imgui_Manager::SetProjection_Matrix(glm::mat4 _mat)
{
	Projection = _mat;

}

void ys::Imgui_Manager::SetObject(ys::GameObject* Game_Object)
{
	Object_Pointer = Game_Object;
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

	

	if (Object_Pointer != nullptr && iGizmo_type != -1) // ��ü�� �޾ƿͼ� ��ü�� �ִٸ�? �˻�
	{
		// gizmos
		ImGuizmo::SetOrthographic(false); // ���� ������ �ϰ� �����ϱ� ���� ����
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();


		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight);

		// ī�޶� �����;� ��

		glm::mat4 Object_Matrix = Object_Pointer->GetComponent<Transform>()->GetWorldMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(CameraMatrix), glm::value_ptr(Projection),
			(ImGuizmo::OPERATION)iGizmo_type, ImGuizmo::LOCAL, glm::value_ptr(Object_Matrix));

		if (ImGuizmo::IsUsing())
		{
			float translation[3], rotation[3], scale[3];

			float ObjMatrix[4][4] = {};

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					ObjMatrix[i][j] = Object_Matrix[i][j];
				}
			}

			ImGuizmo::DecomposeMatrixToComponents(&ObjMatrix[0][0], translation, rotation, scale);


			// ȸ��
			glm::vec3 Origin_Rotation = Object_Pointer->GetComponent<Transform>()->GetRotation(); // ���� ���� ȸ�� ��
			glm::vec3 Delta_Rotation = glm::vec3(rotation[0], rotation[1], -rotation[2]) - Origin_Rotation; // ȸ�� ��ȭ�� // z �� ������ ���������� ȸ���� �̻��ϰ� ��
			if (Delta_Rotation[0] != 0.0f)
			{
				int i = 5;
			}

			Origin_Rotation += Delta_Rotation; // ��ȭ�� ���ϱ�

			Object_Pointer->GetComponent<Transform>()->SetPosition(glm::vec3(translation[0], translation[1], translation[2]));
			Object_Pointer->GetComponent<Transform>()->SetRotation(Origin_Rotation);
			Object_Pointer->GetComponent<Transform>()->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
		}
	}

}
