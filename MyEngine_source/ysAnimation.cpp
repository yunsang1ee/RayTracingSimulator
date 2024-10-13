#include "ysAnimation.h"
#include <ysTimer.h>
#include <ysAnimator.h>
#include <ysGameObject.h>
#include <ysTransform.h>
#include <ysRenderer.h>

namespace ys
{
	Animation::Animation() : Resource(enums::ResourceType::Animation)
		, animator(nullptr), texture(nullptr), animationSheet{}, index(-1), time(0.0f), isComplete(false)
	{
	}
	Animation::~Animation()
	{
	}
	HRESULT Animation::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	void Animation::Upadate()
	{
		if (isComplete) return;
		time += Timer::getDeltaTime();
		if (animationSheet[index].duration <= time)
		{
			time = 0.0f;
			if (index < animationSheet.size() - 1)
				index++;
			else
				isComplete = true;
		}
	}
	void Animation::Render(HDC hDC)
	{
		if (!texture)
			return;
		
		GameObject* gmaeObj = animator->GetOwner();
		auto tr = gmaeObj->GetComponent<Transform>();
		Vector2 position = tr->GetPosition();
		Vector2 scale = tr->GetScale();
		float rotation = tr->GetRotation();

		if (renderer::mainCamera)
			position = renderer::mainCamera->CalculatPosition(position);

		Sprite sprite = animationSheet[index];

		auto type = texture->GetTextureType();
		if (type == graphics::Texture::TextureType::Bmp)
		{
			TransparentBlt(hDC
				, position.x + sprite.offset.x
				, position.y + sprite.offset.y
				, sprite.size.x * scale.x
				, sprite.size.y * scale.y
				, texture->GetDC()
				, sprite.leftTop.x
				, sprite.leftTop.y
				, sprite.size.x
				, sprite.size.y
				, RGB(255, 0, 255));
			
			//BLENDFUNCTION bf;
			//bf.AlphaFormat = AC_SRC_ALPHA;
			//bf.BlendFlags = 0;
			//bf.BlendOp = AC_SRC_OVER;
			//bf.SourceConstantAlpha = 255;//추후 투명이 필요하면 변수 추가
			//
			//AlphaBlend(hDC, position.x, position.y, sprite.size.x, sprite.size.y
			//	, texture->GetDC(), sprite.leftTop.x, sprite.leftTop.y
			//	, texture->GetWidth() - sprite.offset.x, texture->GetHeight() - sprite.offset.y, bf);
		}
		else if (type == graphics::Texture::TextureType::Png)
		{
			Gdiplus::Graphics graphics(hDC);
			Gdiplus::ImageAttributes imageAttr;
			imageAttr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));

			graphics.TranslateTransform(position.x, position.y);
			graphics.RotateTransform(rotation);
			graphics.TranslateTransform(-position.x, -position.y);

			graphics.DrawImage(texture->GetImage()
				, Gdiplus::Rect(position.x + sprite.offset.x
					, position.y + sprite.offset.y
					, sprite.size.x * scale.x
					, sprite.size.y * scale.y)
				, sprite.leftTop.x
				, sprite.leftTop.y
				, sprite.size.x
				, sprite.size.y
				, Gdiplus::UnitPixel
				, &imageAttr);
		}
	}
	void Animation::CrateAnimation(const std::wstring& name, graphics::Texture* spriteSheet
		, const math::Vector2& leftTop, const math::Vector2& size, const math::Vector2& offset
		, const UINT lenth, const float duration, const bool& isReverse)
	{
		setName(name);
		texture = spriteSheet;
		for (size_t i = 0; i < lenth; i++)
		{
			Sprite sprite{};
			sprite.leftTop.x = leftTop.x + size.x * i;
			sprite.leftTop.y = leftTop.y;
			sprite.size = size;
			sprite.offset = offset;
			sprite.duration = duration;
			animationSheet.push_back(sprite);
		}

		if (isReverse)
		{
			for(auto& sprite : animationSheet)
			{
				StretchBlt(texture->GetDC()
					, sprite.leftTop.x + sprite.size.x
					, sprite.leftTop.y
					, -sprite.size.x
					, sprite.size.y
					, texture->GetDC()
					, sprite.leftTop.x
					, sprite.leftTop.y
					, sprite.size.x
					, sprite.size.y
					, SRCCOPY);
			}
		}
	}
	void Animation::Reset()
	{
		time = 0.0f;
		index = 0;
		isComplete = false;
	}
}