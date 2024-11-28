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

		void Init(HWND, GLFWwindow* window, RECT screenSize, bool isWinAPI = true);
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void Release();

		void setScreen(RECT screen);

		HWND getHWND() { return hWnd; }
		GLFWwindow*  getWindow() { return window; }
		HDC getHDC() { return hDc; }
		POINT getScreen() { return screen; }
		glm::vec2 getScreenf() { return glm::vec2(static_cast<float>(screen.x), static_cast<float>(screen.y)); }

	private:
		HWND hWnd;
		GLFWwindow* window;
		HDC hDc;
		HDC hBackDc;
		HBITMAP hBitmap;
		POINT screen;
		bool isWinAPI;
		float frameCheck;
	};
}