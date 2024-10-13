#pragma once
#include "ysEntity.h"

namespace ys
{
	class Resource : public Entity
	{
	public:
		Resource(const enums::ResourceType& type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetPath() { return path; }
		void SetPath(const std::wstring& path) { this->path = path; }

	private:
		enums::ResourceType type;
		std::wstring path;
	};
}
