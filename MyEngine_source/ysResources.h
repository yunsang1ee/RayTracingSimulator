#pragma once
#include "ysResource.h"
#include <assert.h>

namespace ys
{
	class Resources
	{
	public:
		template <typename T>
		static T* Find(const std::wstring key)
		{
			auto iter = resources.find(key);
			if (iter == resources.end())
				return nullptr;
			return dynamic_cast<T*>(iter->second);
		}

		template <typename T>
		static T* Load(const std::wstring key, const std::wstring path)
		{
			T* resource = Resources::Find<T>(key);
			if (resource != nullptr)
				return resource;

			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
			}

			resource->setName(key);
			resource->SetPath(path);
			resources.insert(std::make_pair(key, resource));

			return resource;
		}

		static void Release()
		{
			for (auto resource : resources)
			{
				delete resource.second;
				resource.second = nullptr;
			}
		}

	private:
		static std::map<std::wstring, Resource*> resources;
	};
}

