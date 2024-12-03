#pragma once
#include <ysScene.h>

namespace ys
{
	class PlanetaryScene : public Scene
	{
	public:
		PlanetaryScene();
		~PlanetaryScene();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC, const int& index);
		virtual void Destroy();

		virtual void OnEnter(); //title -> play scene (play init)
		virtual void OnExit(); //title -> play scene (title exit)

	private:
		GLuint VAO;

		GLuint axisVBO;

		GameObject* mainObject;
	};
}

/*
	알베도		-> 텍스처
	노멀맵		-> 텍스처	
	하이트맵	-> 텍스처
	셰도우맵	-> 텍스처
*/