#pragma once
#include <ysComponent.h>
#include <ysGameObject.h>
#include <ysLayer.h>
#include <ysSceneManager.h>
#include <ysScene.h>
#include <ysTransform.h>

namespace ys::object
{  
	template <typename T>
	static T* Instantiate(const enums::LayerType& type)
	{
		T* gameObject = new T();
		auto activeScene = SceneManager::GetActiveScene(); //현재 액티브씬의
		auto layer = activeScene->GetLayer(type); //해당하는 레이어에
		gameObject->SetLayerType(type);
		layer->AddGameObject(gameObject); //오브젝트를 추가한다
		return gameObject;
	}

	template <typename T>
	static T* Instantiate(const enums::LayerType& type, const Vector2& position)
	{
		T* gameObject = new T();
		auto activeScene = SceneManager::GetActiveScene(); //현재 액티브씬의
		auto layer = activeScene->GetLayer(type); //해당하는 레이어에
		gameObject->SetLayerType(type);
		layer->AddGameObject(gameObject); //오브젝트를 추가한다

		auto tr = gameObject->GetComponent<Transform>();
		tr->SetPosition(position);

		return gameObject;
	}
	template <typename T>
	static T* Instantiate(const enums::LayerType& type, const Vector2& position, const float& rotation)
	{
		T* gameObject = new T();
		auto activeScene = SceneManager::GetActiveScene(); //현재 액티브씬의
		auto layer = activeScene->GetLayer(type); //해당하는 레이어에
		gameObject->SetLayerType(type);
		layer->AddGameObject(gameObject); //오브젝트를 추가한다

		auto tr = gameObject->GetComponent<Transform>();
		tr->SetPosition(position);
		tr->SetRotation(rotation);

		return gameObject;
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		Scene* activeScene = SceneManager::GetActiveScene();
		activeScene->EraseGameObject(gameObject);
		SceneManager::GetAbsoluteScene()->AddGameObject(gameObject, gameObject->GetLayerType());
	}

	static void Destroy(GameObject* object) { if(object != nullptr) object->death(); }
}