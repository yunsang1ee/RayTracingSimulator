#pragma once
#include <ysScene.h>

namespace ys
{
	class PlanetaryScene : public Scene
	{
	public:
		PlanetaryScene();
		~PlanetaryScene();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC, const int& index);
		virtual void Destroy();

		virtual void OnEnter(); //title -> play scene (play init)
		virtual void OnExit(); //title -> play scene (title exit)

		void SetUpFBO(int iX, int iY);
		void RenderTo_Imgui_FBO(HDC hDC, const int& index);

	private:
		GLuint VAO;

		GLuint axisVBO;

		GameObject* mainObject;


	private:
		GLuint Imgui_fbo, Imgui_fboTexture;

		int iImguiView_X = 1920;
		int iImguiView_Y = 1080;

		int iToolSize_X = 800;
		int iToolSize_Y = 600;

	};
}

/*
	�˺���		-> �ؽ�ó
	��ָ�		-> �ؽ�ó	
	����Ʈ��	-> �ؽ�ó
	�ε����	-> �ؽ�ó
*/