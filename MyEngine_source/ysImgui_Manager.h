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

		static void Release(); // imgui에서 사용된 변수 해제할 부분

	public:
		static void Init(GLFWwindow* window); // 초기화

		static void Update(); // 실질적인 부분

		static void LateUpdate();

		static void Render(); // 그리기

		static void Destroy(); // 가장 마지막에 불려야 할 것 imgui를 닫아버림

		static void GetFBO(GLuint fbo, GLuint fboTexture);


	private: // 나중에 여기에 변수 추가
		static Imgui_Manager* imgui_Manager;

		static GLuint fbo, fboTexture;

		static int iSubView_X;
		static int iSubView_Y;

	};
}