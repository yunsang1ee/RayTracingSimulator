#pragma once
#include "ysComponent.h"
#include "ysTexture.h"

namespace ys
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hDC) override;

		math::Vector2 GetSize() { return size; }
		math::Vector2 GetOffsetLT() { return offsetLT; }
		math::Vector2 GetOffsetRB() { return offsetRB; }

		graphics::Texture* GetTexture() { return texture; }

		void SetTexture(graphics::Texture* texture) { this->texture = texture; }
		void SetOffset(const math::Vector2& LT, const math::Vector2& RB = math::Vector2::Zero)
		{
			this->offsetLT = LT;
			this->offsetRB = RB;
		}
		void SetSizeByTexture(const math::Vector2& size) { this->size = size; }
		void SetSizeByScreen(const math::Vector2& size)
		{
			this->size.x = size.x / texture->GetWidth();
			this->size.y = size.y / texture->GetHeight();
		}

	private:
		graphics::Texture* texture;
		math::Vector2 size;
		math::Vector2 offsetLT;
		math::Vector2 offsetRB;
	};
}