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

		static void SetObject(ys::GameObject* Game_Object);

		static float Check_Object(ys::GameObject* Game_Object);



		// ���� �� �ȼ�
		static UINT Get_RayPerPixel() { return rayPerPixel; }
		static UINT Get_MaxBounceCount() { return maxBounceCount; }
		static UINT Get_Dispath() { return Dispath; }

		// fov
		static float Get_Fov() { return glm::radians(fov); }





		static void Test_Object(ys::GameObject* Game_Object);

		static GameObject* GetPickedObject() { return Object_Pointer; }



		static float RaySphere(Ray ray, glm::vec3 sphereCenter, float sphereRadius);

		static bool isGizmoUsing();


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
		static int iPhongView_X;
		static int iPhongView_Y;

		static ys::GameObject* Main_Camera; // ī�޶� ���

		static ys::GameObject* Object_Pointer; // ������ ��ü ���

		static glm::vec3 CameraPos; // ���� ī�޶� ��ġ


		static bool Drop_Object; // ���� ��ŷ�� ������Ʈ�� �ٲ� �غ� �Ǿ�����?


		// �ȼ��� ���� ��
		static UINT rayPerPixel;
		static UINT Dispath;
		static UINT maxBounceCount;

		// fov

		static float fov;


	};
}