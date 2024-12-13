#include "ysTimer.h"
#include "ysInputManager.h"

namespace ys
{
	LARGE_INTEGER Timer::cpuFrequency{};
	LARGE_INTEGER Timer::prevFrequency{};
	LARGE_INTEGER Timer::curFrequency{};
	float Timer::deltaTime{};
	float Timer::runTime{};
	float Timer::curTime{};

	float Timer::FPS{};
	float Timer::curFPS{};
	float Timer::targetFPS{144};

	void ys::Timer::Init()
	{
		QueryPerformanceFrequency(&cpuFrequency);
		QueryPerformanceCounter(&prevFrequency);
	}

	void ys::Timer::Update()
	{
		QueryPerformanceCounter(&curFrequency);
		float diffFrequency = static_cast<float>(curFrequency.QuadPart - prevFrequency.QuadPart);
		deltaTime = diffFrequency / static_cast<float>(cpuFrequency.QuadPart);

		if(targetFPS > 0.0f)
		{
			while (deltaTime < 1.0f / targetFPS)
			{
				QueryPerformanceCounter(&curFrequency);
				float diffFrequency = static_cast<float>(curFrequency.QuadPart - prevFrequency.QuadPart);
				deltaTime = diffFrequency / static_cast<float>(cpuFrequency.QuadPart);
			}
		}
		prevFrequency.QuadPart = curFrequency.QuadPart;
		runTime += deltaTime;
		
		FPS++;
		curTime += deltaTime;
		if (curTime > 1.0f)
		{
			curFPS = FPS;
			FPS = 0;
			curTime = 0;
		}

		if (InputManager::getKeyDown(GLFW_KEY_SCROLL_LOCK) && InputManager::getKey(VK_CONTROL))
			targetFPS = (targetFPS == 0) ? 144 : 0;
	}
	void Timer::Render(HDC hDC, POINT screen)	
	{
		std::wstring text;
		text += L"FPS: " + std::to_wstring(static_cast<int>(1 / deltaTime)) + L", lockFPS: " + std::to_wstring(getRealFPS()) +
			L", runTime: " + std::to_wstring(runTime);
		SIZE size;  GetTextExtentPoint32(hDC, text.c_str(), text.size(), &size);
		TextOut(hDC, screen.x - size.cx, 0, text.c_str(), text.size());
	}

}

