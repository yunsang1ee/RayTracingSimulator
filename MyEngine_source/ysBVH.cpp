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
		BoundingBox BVH{};

		for (int i = 0; i < vertices.size(); ++i)
		{
			BVH.GrowToInclude(vertices[i].position);
		}

		//std::vector<Triangle> triangles{};


		for (int i = 0; i < indices.size(); ++i)
		{
			// 0
			graphics::Vertex a = vertices[indices[i++]];
			// 1
			graphics::Vertex b = vertices[indices[i++]];
			// 2
			graphics::Vertex c = vertices[indices[i]];

			glm::vec3 center = glm::vec3((a.position + b.position + c.position) / 3.f);

			AllTriangle.push_back(Triangle(a, b, c, center));

		}

		Root.BVH = BVH;
		Root.TriangleCount = AllTriangle.size();

		// 가장 처음
		Root.TriangleIndex = 0;

		AllNode.push_back(Root);

		Split(0);
	}

	void BVH::Split(UINT parent, int depth)
	{
		if (AllNode[parent].TriangleCount <= MAX_BVH_TRIANGLE_NUM)
		{
			return;
		}

		AllNode[parent].ChildIndex = AllNode.size();



		Node ChildA{};
		Node ChildB{};

		ChildA.TriangleIndex = AllNode[parent].TriangleIndex;
		ChildB.TriangleIndex = AllNode[parent].TriangleIndex;


		// ChooseSplit
		glm::vec3 Size = AllNode[parent].BVH.Size;
		int splitAxis = Size.x > glm::max(Size.y, Size.z) ? 0 : Size.y > Size.z ? 1 : 2;

		float BVH_Center = AllNode[parent].BVH.Center[splitAxis];

		for (int i = AllNode[parent].TriangleIndex; i < AllNode[parent].TriangleIndex + AllNode[parent].TriangleCount; ++i)
		{

			// 부모의 바운딩볼륨 중심보다 작은지 판단?
			bool inA = AllTriangle[i].Center[splitAxis] < BVH_Center;

			Node& Child = inA ? ChildA : ChildB;

			Child.BVH.GrowToInclude(AllTriangle[i]);
			Child.TriangleCount++;

			if (inA) // 부모의 바운딩볼륨 중심보다 작다면?
			{
				int swap = Child.TriangleIndex + Child.TriangleCount - 1;
				Triangle chagne = AllTriangle[i];
				AllTriangle[i] = AllTriangle[swap];
				AllTriangle[swap] = chagne;

				//(AllTriangle[i], AllTriangle[swap]) = (AllTriangle[swap], AllTriangle[i]);

				ChildB.TriangleIndex++;

			}


		}

		AllNode.push_back(ChildA);
		AllNode.push_back(ChildB);

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

		std::cout << "BV_size : " << AllNode[node].BVH.Size.x<< ", " << AllNode[node].BVH.Size.y << ", " << AllNode[node].BVH.Size.z << std::endl;

		if (AllNode[node].ChildIndex == 0)
		{
			return;
		}

		DrawNodes(AllNode[node].ChildIndex, depth + 1);

		DrawNodes(AllNode[node].ChildIndex + 1, depth + 1);

	}

}