#include "YSapplication.h"
#include "ysSceneManager.h"
#include "ysInputManager.h"
#include "ysTimer.h"
#include <ysResources.h>
#include <ysCollisionManager.h>
//#include "ysSoundManager.h"

namespace ys
{
	Application::Application() : hWnd(nullptr), window(nullptr), hDc(nullptr), hBackDc(nullptr), hBitmap(nullptr), screen{}, isWinAPI{}, frameCheck{} {}

	Application::~Application()
	{
	}

	void Application::Init(HWND hWnd_, GLFWwindow* window, RECT screenSize, bool isWinAPI)
	{
		this->isWinAPI = isWinAPI;
		hWnd = hWnd_;
		if (isWinAPI)
		{
			hDc = GetDC(hWnd_);
			hBackDc = CreateCompatibleDC(hDc);
		}
		else
		{
			this->window = window;
		}
			
		setScreen(screenSize);

		//SoundManager::Init();
		InputManager::Init();
		Timer::Init();

		CollisionManager::Init();
		SceneManager::Init();
	}

	void Application::Run()
	{
		InputManager::BeforeUpdate();
		Timer::Update();
		Update();
		LateUpdate();
		Render();

		Destroy();
		InputManager::AfterUpdate();
	}

	void Application::Update()
	{
		CollisionManager::Update();
		SceneManager::Update();
		//SoundManager::Update();
	}	

	void Application::LateUpdate()
	{
		CollisionManager::LateUpdate();
		SceneManager::LateUpdate();
	}

	void Application::Render()
	{
		if(isWinAPI) PatBlt(hBackDc, 0, 0, screen.x, screen.y, WHITENESS);
		else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CollisionManager::Render(hBackDc);
		SceneManager::Render(hBackDc);

		glfwSetWindowTitle(window, (std::to_string(screen.x)
			+ std::string(", ")
			+ std::to_string(screen.y)
			+ std::string(" | ")
			+ std::to_string(1  / Timer::getDeltaTime())
			).c_str());

		//Timer::Render(hBackDc, screen);
		if(isWinAPI) BitBlt(hDc, 0, 0, screen.x, screen.y, hBackDc, 0, 0, SRCCOPY);
		else glfwSwapBuffers(window);
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Release()
	{
		SceneManager::Release();
		Resources::Release();
		//SoundManager::Release();
	}

	void Application::setScreen(RECT screenSize)
	{
		if (isWinAPI) ::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);

		screen.x = screenSize.right - screenSize.left; screen.y = screenSize.bottom - screenSize.top;

		if (isWinAPI) 
		{
			SetWindowPos(hWnd, nullptr, 0, 0, screen.x, screen.y, 0);

			hBitmap = CreateCompatibleBitmap(hDc, screen.x, screen.y);
			auto oldBitmap = SelectObject(hBackDc, hBitmap);
			DeleteObject(oldBitmap);
		}
		else
			glViewport(0, 0, screen.x, screen.y);
	}
}