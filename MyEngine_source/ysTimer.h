#pragma once
#include "CommonInclude.h"

namespace ys
{
	class Timer
	{
	public:
		static void Init();
		static void Update();
		static void Render(HDC hDC, POINT screen);

		inline static float getDeltaTime() { return deltaTime; }
		static float getFrame() { return FPS; }

		//(1 / deltaTime)
		static void setTargetFPS(float value) { targetFPS = value; }
		static float getRealFPS()
		{
			if (targetFPS > 0.0f)
				if ((1 / deltaTime) / ((1 / deltaTime) / targetFPS) < (1 / deltaTime))
					return (1 / deltaTime) / ((1 / deltaTime) / targetFPS);
				else
					return (1 / deltaTime);
			else
				return 0.0f;
		}

	private:
		static LARGE_INTEGER cpuFrequency;
		static LARGE_INTEGER prevFrequency;
		static LARGE_INTEGER curFrequency;
		static float deltaTime;
		static float runTime;
		static float curTime;

		static float targetFPS;
		static float FPS;
		static float curFPS;
	};
}