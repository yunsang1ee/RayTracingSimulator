#pragma once
#include "CommonInclude.h"


namespace ys
{
	class Imgui_Manager
	{
	public:
		Imgui_Manager();
		~Imgui_Manager();


	public:
		static Imgui_Manager* Get_Imgui_Manager()
		{
			if (imgui_Manager == nullptr)
			{
				Create_Imgui_Manager();
			}
			else
			{
				return imgui_Manager;
			}
		}


	private:
		static void Create_Imgui_Manager()
		{
			imgui_Manager = new Imgui_Manager;
		}

		static void Release(); // imgui���� ���� ���� ������ �κ�

	public:
		static void Init(GLFWwindow* window); // �ʱ�ȭ
		static void PostInit(); // �ʱ�ȭ

		static void Update(); // �������� �κ�

		static void LateUpdate();

		static void Render(); // �׸���

		static void Destroy(); // ���� �������� �ҷ��� �� �� imgui�� �ݾƹ���

		static void SetFBO(GLuint fboTexture);

		static void SetFBO_Two(GLuint fboTexture);

		static void SetCamera_Matrix(glm::mat4 _mat);
		static void SetProjection_Matrix(glm::mat4 _mat);
		static void SetObject(ys::GameObject* Game_Object);


	private:
		static void Change_Transform_Object();


	private: // ���߿� ���⿡ ���� �߰�
		static Imgui_Manager* imgui_Manager;

		static GLuint sourceFramebuffer, targetFramebuffer;
		static GLuint phongTexture;
		static GLuint raytracingTexture;
		static GLuint resizeTexture;


		// Gizmo type

		static int iGizmo_type;

		//static void* Camera;
		static int iSubView_X;
		static int iSubView_Y;

		static glm::mat4 CameraMatrix; // ī�޶� ���
		static glm::mat4 Projection; // ���� ���
		static ys::GameObject* Object_Pointer; // ������ ��ü ���


	};
}