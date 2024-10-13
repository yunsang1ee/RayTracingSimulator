#include "ysTexture.h"
#include "YSapplication.h"

extern ys::Application app;

namespace ys::graphics
{
	Texture::Texture() 
		: Resource(enums::ResourceType::Texture)
	{
	}

	Texture::~Texture()
	{
	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		//bmp
		std::wstring ext = path.substr(path.find_last_of(L".") + 1);
		//find_last_of는 해당 find의 값의 위치 그니까 거기에 +1 그 이후부터 마지막까지를 substr하겠는다는거임 ㅇㅋ?
		if (ext == L"bmp")
		{
			type = TextureType::Bmp;
			bitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (bitmap == nullptr)
				return S_FALSE;

			BITMAP info{};
			GetObject(bitmap, sizeof(BITMAP), &info);
			width = info.bmWidth;
			height = info.bmHeight;

			if (info.bmBitsPixel == 32)
				isAlpha = true;
			else if (info.bmBitsPixel == 24)
				isAlpha = false;

			HDC mainDC = app.getHDC();
			hdc = CreateCompatibleDC(mainDC);
			SelectObject(hdc, bitmap);
		}
		else if (ext == L"png")
		{
			type = TextureType::Png;
			image = Gdiplus::Image::FromFile(path.c_str());
			width = image->GetWidth();
			height = image->GetHeight();
		}
		//png
	/*	*/
		return S_OK;
	}
}
