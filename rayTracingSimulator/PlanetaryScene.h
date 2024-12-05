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
		void RenderTo_Imgui_FBO_Two(HDC hDC, const int& index);

	private:
		GLuint VAO {};

		GLuint axisVBO {};

		GameObject* mainObject {};


	private:
		GLuint Imgui_fbo {}, Imgui_fboTexture {};
		GLuint Imgui_fbo_Two {}, Imgui_fboTexture_Two {};

		int iImguiView_X = 1920;
		int iImguiView_Y = 1080;

		int iToolSize_X = 1920;
		int iToolSize_Y = 1080;

	};
}

/*
	알베도		-> 텍스처
	노멀맵		-> 텍스처	
	하이트맵	-> 텍스처
	셰도우맵	-> 텍스처
*/