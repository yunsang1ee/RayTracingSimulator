#pragma once
#include "ysCollider.h"
#include <ysGraphics.h>
constexpr uint32_t kLeafMax = 12;
constexpr uint32_t kInvalid = std::numeric_limits<uint32_t>::max();

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
			glm::vec3 Center = {};
			glm::vec3 HalfExtent = {};

			void Recalc()
			{
				Center = (Min + Max) * 0.5f;
				HalfExtent = glm::abs(Max - Min) * 0.5f;
			}

			void GrowToInclude(const glm::vec3& p)
			{
				Min = glm::min(Min, p);
				Max = glm::max(Max, p);
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
			BoundingBox Box{};
			uint32_t TriangleIndex{}; // 삼각형이 시작되는 인덱스?
			uint32_t TriangleCount{}; // 삼각형을 몇 개 가지고 있는지?
			uint32_t ChildIndex{ kInvalid }; // 자식
			// 오른자식은 ChildIndex+1
		};


		void Input_BVH(std::vector<graphics::Vertex>& vertices, std::vector<unsigned int>& indices);

		void Split(uint32_t parent, int depth);

		Node* Get_Root() { return &Root; }

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

		void DrawNodes(UINT node = 0, int depth = 0);

		std::vector<graphics::GPUBVHNode> ConvertToGPUNodes() const;
		std::vector<graphics::PackedVertex> GetTriangleVertices() const;

	private:

		Node Root{};

		// 

		std::vector<Node> AllNode{};


		std::vector<Triangle> AllTriangle{};
	};
}

