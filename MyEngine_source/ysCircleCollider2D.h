#pragma once
#include "ysCollider.h"

namespace ys
{
	class CircleCollider2D : public Collider
	{
	public:
		CircleCollider2D();
		~CircleCollider2D();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

	private:

	};
}

