#include "ysSceneManager.h"
#include "ysAbsoluteScene.h"

namespace ys
{
	std::map<std::wstring, Scene*> SceneManager::Scenes;
	Scene* SceneManager::activeScene;
	Scene* SceneManager::absoluteScene;

	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		if (activeScene)
			activeScene->OnExit();
		auto iter = Scenes.find(name);
		if (iter == Scenes.end())
			return nullptr;

		activeScene = iter->second;
		activeScene->OnEnter();
		return activeScene;
	}

	std::vector<GameObject*> SceneManager::GetGameObejcts(const LayerType& layer)
	{
		std::vector<GameObject*> gameObjects
			= activeScene->GetLayer(layer)->GetGameObjects();
		std::vector<GameObject*> absoluteGameObjects
			= absoluteScene->GetLayer(layer)->GetGameObjects();
		gameObjects.insert(gameObjects.end()
			, absoluteGameObjects.begin()
			, absoluteGameObjects.end());

		return gameObjects;
	}

	void SceneManager::Init()
	{
		absoluteScene = CreateScene<AbsoluteScene>(L"Absolute");
	}
	void SceneManager::Update()
	{
		activeScene->Update();
		absoluteScene->Update();
	}
	void SceneManager::LateUpdate()
	{
		activeScene->LateUpdate();
		absoluteScene->LateUpdate();
	}
	void SceneManager::Render(HDC hDC)
	{
		for(int layerIndex = 0; layerIndex < (UINT)LayerType::Max; ++layerIndex)
		{
			activeScene->Render(hDC, layerIndex);
			absoluteScene->Render(hDC, layerIndex);
		}
	}
	void SceneManager::Destroy()
	{
		activeScene->Destroy();
		absoluteScene->Destroy();
	}
	void SceneManager::Release()
	{
		for (auto Scene : Scenes)
		{
			delete Scene.second;
			Scene.second = nullptr;
		}
	}
}