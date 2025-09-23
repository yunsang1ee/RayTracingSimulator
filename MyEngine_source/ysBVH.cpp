#include "ysBVH.h"
#include "ysGameObject.h"
#include <ysTransform.h>
#include <ysRenderer.h>
#include <iostream>

namespace ys
{
	BVH::BVH() : Collider(enums::ColliderType::Box2D)
	{
	}
	BVH::~BVH()
	{
	}
	void ys::BVH::Input_BVH(std::vector<graphics::Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		AllTriangle.clear();
		AllNode.clear();

		BoundingBox rootBox{};
		for (int i = 0; i < vertices.size(); ++i)
		{
			rootBox.GrowToInclude(vertices[i].position);
		}
		rootBox.Recalc();

		AllTriangle.reserve(indices.size() / 3);
		for (size_t i = 0; i + 2 < indices.size(); i += 3)
		{
			const graphics::Vertex a = vertices[indices[i + 0]];
			const graphics::Vertex b = vertices[indices[i + 1]];
			const graphics::Vertex c = vertices[indices[i + 2]];

			glm::vec3 center = (a.position + b.position + c.position) / 3.f;
			AllTriangle.emplace_back(a, b, c, center);
		}

		Root = {
			.Box = rootBox,
			.TriangleIndex = 0,
			.TriangleCount = static_cast<uint32_t>(AllTriangle.size()),
			.ChildIndex = kInvalid
		};
		AllNode.push_back(Root);

		Split(0, 0);
	}

	void BVH::Split(uint32_t parent, int depth)
	{
		Node& p = AllNode[parent];

		if (p.TriangleCount <= kLeafMax)
		{
			p.ChildIndex = kInvalid;
			return;
		}

		const glm::vec3 he = p.Box.HalfExtent;
		int axis = (he.x > he.y && he.x > he.z) ? 0 : (he.y > he.z ? 1 : 2);
		const float splitPos = p.Box.Center[axis];

		const uint32_t begin = p.TriangleIndex;
		const uint32_t end = begin + p.TriangleCount;

		auto midIter = std::partition(
			AllTriangle.begin() + begin,
			AllTriangle.begin() + end,
			[axis, splitPos](const Triangle& tri)
			{
				return tri.Center[axis] < splitPos;
			}
		);
		uint32_t mid = static_cast<uint32_t>(std::distance(AllTriangle.begin(), midIter));

		uint32_t leftCount = mid - begin;
		uint32_t rightCount = end - mid;

		if (leftCount == 0 || rightCount == 0)
		{
			const uint32_t halfCount = p.TriangleCount / 2;
			std::nth_element(
				AllTriangle.begin() + begin,
				AllTriangle.begin() + begin + halfCount,
				AllTriangle.begin() + end,
				[axis](const Triangle& a, const Triangle& b)
				{
					return a.Center[axis] < b.Center[axis];
				}
			);
			leftCount = halfCount;
			rightCount = p.TriangleCount - halfCount;
			mid = begin + leftCount;
		}

		const uint32_t leftIndex = static_cast<uint32_t>(AllNode.size());
		const uint32_t rightIndex = leftIndex + 1;
		p.ChildIndex = leftIndex;

		Node left{}, right{};
		left.TriangleIndex = begin;
		left.TriangleCount = leftCount;
		right.TriangleIndex = mid;
		right.TriangleCount = rightCount;

		for (size_t i = left.TriangleIndex; i < left.TriangleIndex + left.TriangleCount; ++i)
		{
			left.Box.GrowToInclude(AllTriangle[i]);
		}
		left.Box.Recalc();
		for (size_t i = right.TriangleIndex; i < right.TriangleIndex + right.TriangleCount; ++i)
		{
			right.Box.GrowToInclude(AllTriangle[i]);
		}
		right.Box.Recalc();

		AllNode.push_back(left);
		AllNode.push_back(right);

		Split(AllNode[parent].ChildIndex, depth + 1);
		Split(AllNode[parent].ChildIndex + 1, depth + 1);
	}


	void BVH::Init()
	{
	}
	void BVH::Update()
	{
	}
	void BVH::LateUpdate()
	{
	}
	void BVH::Render(HDC hDC)
	{

	}



	void BVH::DrawNodes(UINT node, int depth)
	{
		// 테스트용 렌더에서 호출함

		if (node >= AllNode.size())
		{
			return;
		}


		std::cout << "\ndepth" << depth << std::endl;

		std::cout << "triangle Num : " << AllNode[node].TriangleCount<< std::endl;

		std::cout << "BV_size : " 
			<< AllNode[node].Box.HalfExtent.x
			<< ", " << AllNode[node].Box.HalfExtent.y 
			<< ", " << AllNode[node].Box.HalfExtent.z << std::endl;

		if (AllNode[node].ChildIndex == kInvalid)
		{
			return;
		}

		DrawNodes(AllNode[node].ChildIndex, depth + 1);

		DrawNodes(AllNode[node].ChildIndex + 1, depth + 1);

	}

	std::vector<graphics::GPUBVHNode> BVH::ConvertToGPUNodes() const
	{
		std::vector<graphics::GPUBVHNode> gpuNodes;
		gpuNodes.reserve(AllNode.size());

		for (const auto& node : AllNode)
			gpuNodes.emplace_back(
				node.Box.Min,
				node.ChildIndex,
				node.Box.Max,
				node.TriangleIndex,
				node.TriangleCount
			);

		return gpuNodes;
	}

	std::vector<graphics::PackedVertex> BVH::GetTriangleVertices() const
	{
		std::vector<graphics::PackedVertex> vertices;
		vertices.reserve(AllTriangle.size() * 3);

		for (const auto& triangle : AllTriangle)
		{
			vertices.emplace_back(
				glm::vec4(triangle.VertexA.position, triangle.VertexA.texture.x),
				glm::vec4(triangle.VertexA.normal, triangle.VertexA.texture.x));
			vertices.emplace_back(
				glm::vec4(triangle.VertexB.position, triangle.VertexB.texture.x),
				glm::vec4(triangle.VertexB.normal, triangle.VertexB.texture.x));
			vertices.emplace_back(
				glm::vec4(triangle.VertexC.position, triangle.VertexC.texture.x),
				glm::vec4(triangle.VertexC.normal, triangle.VertexC.texture.x));
		}

		return vertices;
	}

}