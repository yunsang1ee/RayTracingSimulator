#pragma once
#include "CommonInclude.h"
#include "ysEntity.h"
#include "ysGameObject.h"
#include "ysLayer.h"

namespace ys
{
	class Scene : public Entity
	{
	public:

		Scene();
		~Scene();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC, const int& index);
		virtual void Destroy();
		
		virtual void OnEnter(); //title -> play scene (play init)
		virtual void OnExit(); //title -> play scene (title exit)

		void AddGameObject(GameObject* gameObject, const enums::LayerType& type);
		void EraseGameObject(GameObject* gameObject);
		Layer* GetLayer(const enums::LayerType& type) { return layers[static_cast<UINT>(type)]; }

	private:
		std::vector<Layer*> layers;
	};
}