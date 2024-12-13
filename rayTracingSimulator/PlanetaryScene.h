#pragma once
#include <ysScene.h>

namespace ys
{
	class PlanetaryScene : public Scene
	{
		struct Material
		{
			glm::vec4 color;
			glm::vec4 emittedColor;
			glm::vec4 emissionStrength;
		};

		struct Ray
		{
			glm::vec3 origin;
			glm::vec3 dir;
		};

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

		ys::GameObject* RaySphere(GameObject* Object, glm::vec3 Mouse_World_Pos, glm::mat4 viewMatrix, glm::vec3 viewPosition);

	private:
		GLuint VAO{};

		GLuint axisVBO{};
		GLuint quadVBO{};


	private:
		GLuint currentTexture{}, previousTexture{}; //raytarcing(compute shader) not use framebuffer
		GLuint phongFramebuffer{}, depthBuffer{}, phongTexture{};
		//final is default FrameBuffer

		GLuint ssboSphere{};
		std::map<uintptr_t, Sphere> spheres;
		GLuint ssboLight{};

		int iImguiView_X;
		int iImguiView_Y;

		int iToolSize_X;
		int iToolSize_Y;


	private:
		std::vector<GameObject*> AllObject{};


	};
}

/*
	알베도		-> 텍스처
	노멀맵		-> 텍스처
	하이트맵	-> 텍스처
	셰도우맵	-> 텍스처
*/