#pragma once
#include <ysScene.h>

namespace ys
{
	class PlanetaryScene : public Scene
	{
		struct Sphere
		{
			glm::vec3 center;
			float radius;
			Material material;
		};

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

		void PhongRender(HDC hDC, const int& index);
		void SetUpFBO(int iX, int iY);
		void GenObject();
		void UpdateSSBO();

	private:
		GLuint VAO{};

		GLuint axisVBO{};
		GLuint quadVBO{};

		GameObject* mainObject{};

	private:
		GLuint currentTexture{}, previousTexture{}; //raytarcing(compute shader) not use framebuffer
		GLuint phongFramebuffer{}, depthBuffer{}, phongTexture{};
		//final is default FrameBuffer

		std::unordered_map<uintptr_t, std::pair<bool, size_t>> spheresIndex;
		std::vector<Sphere> spheres;
		GLuint ssboSphere;
		size_t ssboSphereSize;
		void* spheresPointer;

		int iImguiView_X;
		int iImguiView_Y;

		int iToolSize_X;
		int iToolSize_Y;

	};
}

/*
	알베도		-> 텍스처
	노멀맵		-> 텍스처
	하이트맵	-> 텍스처
	셰도우맵	-> 텍스처
*/