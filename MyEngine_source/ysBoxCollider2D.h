#pragma once
#include "ysCollider.h"

namespace ys
{
	class BoxCollider2D : public Collider
	{
	public:
		BoxCollider2D();
		~BoxCollider2D();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

	private:

	};
}

