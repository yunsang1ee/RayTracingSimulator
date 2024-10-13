#pragma once
#include "CommonInclude.h"
#include "ysScene.h"
namespace ys
{
	class SceneManager
	{
	public:
		template <typename T>
		static Scene* CreateScene(const std::wstring& name)
		{
			T* scene = new T();
			scene->setName(name);
			activeScene = scene;
			scene->Init();

			Scenes.insert(std::make_pair(name, scene));
			return scene;
		}

		static Scene* LoadScene(const std::wstring& name);
		static Scene* GetActiveScene() { return activeScene; }
		static Scene* GetAbsoluteScene() { return absoluteScene; }
		static std::vector<GameObject*> GetGameObejcts(const LayerType& layer);

		static void Init();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hDC);
		static void Destroy();
		static void Release();

	private:
		static std::map<std::wstring, Scene*> Scenes;
		static Scene* activeScene;
		static Scene* absoluteScene;
	};
}
