#pragma once
#include "ysComponent.h"
namespace ys
{
	using namespace math;
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hDC) override;

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetScale(const Vector2& scale) { this->scale = scale; }
		void SetRotation(const float& rotation) { this->rotation = rotation; }

		Vector2 GetPosition() const { return position; }
		Vector2 GetScale() const { return scale; }
		float GetRotation() const { return rotation; }

	private:
		Vector2 position;
		Vector2 scale;
		float rotation;
	};

}

