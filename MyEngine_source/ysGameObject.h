#pragma once
#include "CommonInclude.h"
#include "ysComponent.h"
using namespace ys::math;
using namespace ys::enums;

namespace ys::object
{
	void Destroy(GameObject* object);
}

namespace ys 
{
	class GameObject
	{
	public:
		friend void object::Destroy(GameObject* object);

		enum class State
		{
			Active,
			Paused,
			Dead,
			End
		};

		GameObject();
		~GameObject();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);
		
		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();	
			comp->SetOwner(this);
			components.at(static_cast<UINT>(comp->GetType())) = comp;
			return comp;
		}

		template <typename T>
		T* GetComponent()
		{
			T* comp = nullptr;
			for(auto component : components)
			{
				comp = dynamic_cast<T*>(component);
				if (comp) break;
			}
			return  comp;
		}

		State GetState() const { return state; }
		LayerType GetLayerType() const { return ownLayer; }
		bool IsActive() const { return state == State::Active; }

		void SetLayerType(const LayerType& layerType) { ownLayer = layerType; }
		void SetActive(const bool& power) { state = power ? State::Active : State::Paused; }

	private:
		void death() { state = State::Dead; }

	private:
		State state;
		std::vector<Component*> components;
		LayerType ownLayer;
	};
}