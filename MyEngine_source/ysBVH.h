#pragma once
#include "ysCollider.h"
#include <ysGraphics.h>
#define MAX_BVH_TRIANGLE_NUM 50

namespace ys
{
	class BVH : public Collider
	{
	public:
		BVH();
		~BVH();

	public:
		struct Triangle
		{
			graphics::Vertex VertexA{};
			graphics::Vertex VertexB{};
			graphics::Vertex VertexC{};

			glm::vec3 Center;
		};

		struct BoundingBox
		{
			glm::vec3 Min = glm::vec3(1.f) * std::numeric_limits<float>::infinity();
			glm::vec3 Max = glm::vec3(1.f) * -std::numeric_limits<float>::infinity();
			glm::vec3 Center = (Min + Max) * 0.5f;
			glm::vec3 Size = abs(Max - Min) / 2.f; // 이거 나중에 생각해봐야 할듯


			void GrowToInclude(glm::vec3 point)
			{
				Min = glm::min(Min, point);
				Max = glm::max(Max, point);
				Center = (Min + Max) * 0.5f;
				Size = abs(Max - Min) / 2.f;
			}

			void GrowToInclude(Triangle triangle)
			{
				GrowToInclude(triangle.VertexA.position);
				GrowToInclude(triangle.VertexB.position);
				GrowToInclude(triangle.VertexC.position);
			}
		};

		struct Node
		{
			BoundingBox BVH{};
			UINT TriangleIndex{}; // 삼각형이 시작되는 인덱스?
			UINT TriangleCount{}; // 삼각형을 몇 개 가지고 있는지?
			UINT ChildIndex{}; // 자식
		};


		void Input_BVH(std::vector<graphics::Vertex>& vertices, std::vector<unsigned int>& indices);

		void Split(UINT parent, int depth = 0);

		Node Get_Root() { return Root; }

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

		void DrawNodes(UINT node = 0, int depth = 0);

	private:

		Node Root{};

		// 

		std::vector<Node> AllNode{};


		std::vector<Triangle> AllTriangle{};

	};
}

