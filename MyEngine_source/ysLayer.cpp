#include "ysLayer.h"
#include "ysGameObject.h"

namespace ys
{
	Layer::Layer() : objects{}
	{
	}

	Layer::~Layer()
	{
		for (auto object : objects)
		{
			delete object;
			object = nullptr;
		}
	}

	void Layer::Init()
	{
		for (auto object : objects)
		{
			if (object == nullptr) continue;
			object->Init();
		}
	}
	void Layer::Update()
	{
		for (GameObject* object : objects)
		{
			if (object == nullptr) continue;

			if (object->IsActive())
				object->Update();
		}
	}
	void Layer::LateUpdate()
	{
		for (auto object : objects)
		{
			if (object == nullptr) continue;

			if (object->IsActive())
				object->LateUpdate();
		}
	}
	void Layer::Render(HDC hDC)
	{
		for (auto object : objects)
		{
			if (object == nullptr) continue;

			if (object->IsActive())
				object->Render(hDC);
		}
	}

	void Layer::Destroy()
	{
		for (auto iter = objects.begin(); iter != objects.end();)
		{
			if (*iter == nullptr) continue;
			auto state = (*iter)->GetState();
			if (state == GameObject::State::Dead)
			{
				auto deleteObj = *iter;
				iter = objects.erase(iter);
				delete deleteObj;
				deleteObj = nullptr;
			}
			else
				++iter;
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr) return;
		objects.push_back(gameObject);
	}
	void Layer::EraseGameObject(GameObject* gameObject)
	{
		//iterator를 안돌아도 삭제가 가능한 STL ㄷㄷ
		std::erase_if(objects,
			[=](GameObject* obj)
			{
				return obj == gameObject;
			});
	}
}