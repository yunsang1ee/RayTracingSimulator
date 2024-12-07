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

	private:
		GLuint VAO{};

		GLuint quadVBO{};

		GameObject* mainObject{};


	private:
		GLuint currentTexture{}, previousTexture{}; //raytarcing(compute shader) not use framebuffer
		GLuint phongFramebuffer{}, phongTexture{};
		GLuint finalFramebuffer{}, finalTexture{};

		int iImguiView_X;
		int iImguiView_Y;

		int iToolSize_X;
		int iToolSize_Y;

		int iFrame = 0; // 현재 멈춘상태로 몇 프레임 돌았는지?

	};
}

/*
	알베도		-> 텍스처
	노멀맵		-> 텍스처
	하이트맵	-> 텍스처
	셰도우맵	-> 텍스처
*/