#pragma once
#include "ysResource.h"
#include "ysTexture.h"

namespace ys
{
	class Animation : public Resource
	{
	public:
		struct Sprite
		{
			math::Vector2 leftTop;
			math::Vector2 size;
			math::Vector2 offset;
			float duration;

			Sprite() : leftTop(math::Vector2::Zero)
				, size(math::Vector2::Zero)
				, offset(math::Vector2::Zero)
				, duration(0.0f) {}
		};

		Animation();
		~Animation();

		HRESULT Load(const std::wstring& path) override;

		void Upadate();
		void Render(HDC hDC);

		void CrateAnimation(const std::wstring& name, graphics::Texture* spriteSheet
			, const math::Vector2& leftTop, const math::Vector2& size, const math::Vector2& offset
			, const UINT lenth, const float duration, const bool& isReverse = false);

		void Reset();

		bool IsComplete() const { return isComplete; }
		void SetAnimatior(class Animator* animator) { this->animator = animator; }

		//박경준 추가
		graphics::Texture* getTexture() { return this->texture; }
		
	private:
		class Animator* animator;
		graphics::Texture* texture;

		std::vector<Sprite> animationSheet;
		int index;
		float time;
		bool isComplete;
		
	};
}
