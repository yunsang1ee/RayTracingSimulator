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

		static void SetFBO_Two(GLuint fboTexture,int iFrame);


	private: // ���߿� ���⿡ ���� �߰�
		static Imgui_Manager* imgui_Manager;

		static GLuint sourceFramebuffer, targetFramebuffer;
		static GLuint phongTexture;
		static GLuint raytracingTexture;
		static GLuint resizeTexture;

		static int iSubView_X;
		static int iSubView_Y;

	};
}