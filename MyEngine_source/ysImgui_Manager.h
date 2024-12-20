#pragma once
#include "CommonInclude.h"
#include "ysGameObject.h"


namespace ys
{
	class Imgui_Manager
	{
	public:
		Imgui_Manager();
		~Imgui_Manager();


	public:

		struct Ray
		{
			glm::vec3 origin;
			glm::vec3 dir;
		};

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

		static void PostInit(); // 초기화

		static void Update(); // 실질적인 부분

		static void LateUpdate();

		static void Render(); // 그리기

		static void Destroy(); // 가장 마지막에 불려야 할 것 imgui를 닫아버림

		static void SetFBO(GLuint fboTexture);

		static void SetFBO_Two(GLuint fboTexture);

		static void SetObject(GameObject* Game_Object);

		static glm::vec3 GenRayDir();
		static float Check_Object(glm::vec3 rayDir, GameObject* Game_Object);


		// 레이 퍼 픽셀
		static UINT Get_RayPerPixel() { return rayPerPixel; }
		static UINT Get_MaxBounceCount() { return maxBounceCount; }
		static UINT Get_Delay() { return delay; }
		static UINT Get_Dispatch() { return dispatch; }
		static bool IsInvironmet() { return isInvironment; }

		// fov
		static float Get_Fov() { return glm::radians(fov); }

		static void Test_Object(GameObject* Game_Object);

		static GameObject* GetPickedObject() { return Object_Pointer; }



		static float RaySphere(Ray ray, glm::vec3 sphereCenter, float sphereRadius);

		static bool isGizmoUsing();

		static bool isObjectChange();

	private:
		static void Change_Transform_Object();


	private: // 나중에 여기에 변수 추가
		static Imgui_Manager* imgui_Manager;

		static GLuint sourceFramebuffer, targetFramebuffer;
		static GLuint phongTexture;
		static GLuint raytracingTexture;
		static GLuint resizeTexture;


		// Gizmo type

		static int iGizmo_type;

		//static void* Camera;
		static int iPhongView_X;
		static int iPhongView_Y;

		static GameObject* Main_Camera; // 카메라 행렬

		static GameObject* Object_Pointer; // 선택한 객체 행렬

		static glm::vec3 CameraPos; // 현재 카메라 위치

		static bool isObjectChanged;
		static bool isObjectScreenHovered;
		static bool isPhongScreenHovered;

		// 픽셀당 레이 수
		static UINT rayPerPixel;
		static UINT dispatch;
		static bool isInvironment;
		static UINT maxBounceCount;
		static UINT delay;

		// fov

		static float fov;


	};
}