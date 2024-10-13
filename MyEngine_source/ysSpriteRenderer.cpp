#include "ysSpriteRenderer.h"
#include "ysGameObject.h"
#include "ysTransform.h"
#include "ysTexture.h"
#include "ysRenderer.h"
#include <cassert>
#include <ysCollider.h>

ys::SpriteRenderer::SpriteRenderer()
	: Component(enums::ComponentType::SpriteRenderer), texture(nullptr)
	, size (Vector2::One), offsetLT(Vector2::Zero), offsetRB(Vector2::Zero)
{
}

ys::SpriteRenderer::~SpriteRenderer()
{
}

void ys::SpriteRenderer::Init()
{
}

void ys::SpriteRenderer::Update()
{
}

void ys::SpriteRenderer::LateUpdate()
{
}

void ys::SpriteRenderer::Render(HDC hDC)
{
	if (texture == nullptr) 
		assert(false, L"File error!");
	auto tr = GetOwner()->GetComponent<Transform>();

	Vector2 position = tr->GetPosition();
	if (renderer::mainCamera)
		position = renderer::mainCamera->CalculatPosition(position);
	Vector2 scale = tr->GetScale();
	float rotation = tr->GetRotation();

	auto cd = GetOwner()->GetComponent<Collider>();
	if(cd != nullptr && cd->isRender())
	{
		return;
	}

	if (texture->GetTextureType() == graphics::Texture::TextureType::Bmp)
	{
		auto check = TransparentBlt(hDC
			, position.x
			, position.y
			, texture->GetWidth() * size.x * scale.x
			, texture->GetHeight() * size.y * scale.y
			, texture->GetDC()
			, offsetLT.x
			, offsetLT.y
			, texture->GetWidth() - offsetLT.x - offsetRB.x
			, texture->GetHeight() - offsetLT.y - offsetRB.y
			, RGB(255, 0, 255));
		assert(check);
	}
	else if (texture->GetTextureType() == graphics::Texture::TextureType::Png)
	{
		Gdiplus::Graphics graphics(hDC);
		Gdiplus::ImageAttributes imageAttr;
		imageAttr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));

		graphics.TranslateTransform(position.x, position.y);
		graphics.RotateTransform(rotation / kPi * 180.0f);
		graphics.TranslateTransform(-position.x, -position.y);

		graphics.DrawImage(texture->GetImage()
			, Gdiplus::Rect(
				position.x
				, position.y
				, texture->GetWidth() * size.x * scale.x
				, texture->GetHeight() * size.y * scale.y)
			, offsetLT.x
			, offsetLT.y
			, texture->GetWidth() - offsetLT.x - offsetRB.x
			, texture->GetHeight() - offsetLT.y - offsetRB.y
			, Gdiplus::UnitPixel, &imageAttr);
	}
}


