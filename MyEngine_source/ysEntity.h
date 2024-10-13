#pragma once
#include "CommonInclude.h"

namespace ys
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		void setName(const std::wstring& nameVal) { name = nameVal; }
		std::wstring& getName() { return name; }

	private:
		std::wstring name;
	};
}

