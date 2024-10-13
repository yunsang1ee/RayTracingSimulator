#pragma once
#include "ysEntity.h"
#include "ysComponent.h"

namespace ys
{
	class Layer : public Entity
	{
	public:
		Layer();
		~Layer();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);
		virtual void Destroy();

		
		void setType(LayerType type) { this->type = type; }//내가 추가함

		void AddGameObject(GameObject* gameObject);
		void EraseGameObject(GameObject* gameObject);
		const std::vector<GameObject*> GetGameObjects() const { return objects; }
		const LayerType getType() const { return type; }//내가 추가함
	private:
		LayerType type;//주석해제함
		std::vector<GameObject*> objects;
	}; 
}

