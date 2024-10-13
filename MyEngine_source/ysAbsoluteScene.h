#pragma once
#include "ysScene.h"

namespace ys
{
	class AbsoluteScene :public Scene
	{
	public:
		AbsoluteScene();
		~AbsoluteScene();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC, const int& index) override;
		virtual void Destroy() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;


	};
}

