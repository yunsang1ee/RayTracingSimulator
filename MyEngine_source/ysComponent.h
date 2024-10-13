#pragma once
#include "ysEntity.h"
using namespace ys::enums;

namespace ys
{
	class GameObject;
	class Component : public Entity
	{
	public:
		Component(ComponentType type);
		~Component();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

		void SetOwner(GameObject* owner) { this->owner = owner; }
		GameObject* GetOwner() const { return owner; }
		ComponentType GetType() const { return myType; }

	private:
		GameObject* owner;
		ComponentType myType;
	};
}

