#pragma once
#include "CommonInclude.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3_loader.h"

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

		static void Update(); // �������� �κ�

		static void LateUpdate();

		static void Render(); // �׸���

		static void Destroy(); // ���� �������� �ҷ��� �� �� imgui�� �ݾƹ���

	private: // ���߿� ���⿡ ���� �߰�
		static Imgui_Manager* imgui_Manager;

	};
}