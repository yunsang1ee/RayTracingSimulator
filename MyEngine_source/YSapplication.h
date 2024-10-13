#pragma once
#include "CommonInclude.h"
#include "ysGameObject.h"

namespace ys
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void Init(HWND, GLFWwindow* window, unsigned int ID, RECT screenSize, bool isWinAPI = true);
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void Release();

		void setScreen(RECT screen);

		HWND getHWND() { return hWnd; }
		GLFWwindow*  getWindow() { return window; }
		unsigned int  getShaderID() { return shaderID; }
		HDC getHDC() { return hDc; }
		POINT getScreen() { return screen; }
		math::Vector2 getScreenf() { return Vector2(static_cast<float>(screen.x), static_cast<float>(screen.y)); }

	private:
		HWND hWnd;
		GLFWwindow* window;
		unsigned int shaderID;
		HDC hDc;
		HDC hBackDc;
		HBITMAP hBitmap;
		POINT screen;
		bool isWinAPI;
		float frameCheck;
	};
}

ys::Application app;