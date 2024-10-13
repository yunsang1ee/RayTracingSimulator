#pragma once
#include "ysResource.h"

namespace ys::graphics
{
	class Texture : public Resource
	{
	public:
		enum class TextureType
		{
			Bmp,
			Png,
			None
		};

		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;

		TextureType GetTextureType() { return type; }
		Gdiplus::Image* GetImage() { return image; }
		HDC GetDC() { return hdc; }
		inline UINT GetWidth() { return width; }
		inline UINT GetHeight() { return height; }
		inline bool IsHeight() { return isAlpha; }

	private:
		bool isAlpha;
		TextureType type;
		//GdiPlus
		Gdiplus::Image* image;
		//Gdi
		HBITMAP bitmap;
		HDC hdc;

		UINT width;
		UINT height;
	};
}
