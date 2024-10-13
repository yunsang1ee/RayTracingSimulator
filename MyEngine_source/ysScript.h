#pragma once
#include "ysComponent.h"

namespace ys
{
	class Script : public Component
	{
	public:
		Script();
		~Script();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC) override;

		virtual void OnCollisionEnter(class Collider* other);
		virtual void OnCollisionStay(class Collider* other);
		virtual void OnCollisionExit(class Collider* other);

	private:

	};

}
