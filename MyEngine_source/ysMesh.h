#pragma once
#include "ysResource.h"
#include "ysGraphics.h"
#define MAX_BVH_TRIANGLE_NUM 50

namespace ys
{
	class Mesh : public Resource
	{
	public:
		struct Data
		{
			Data();
			~Data();
			using UsageType = int;
			UsageType usageType = GL_TRIANGLES;
			std::vector<graphics::Vertex> vertices;
			std::vector<GLuint> indices;
		};
		Mesh();
		virtual ~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVB(std::vector<graphics::Vertex>& vertices);
		bool CreateEB(std::vector<GLuint>& indices);
		GLuint GetIndicesCount() const { return data.indices.size(); };

		void SetUsageType(Data::UsageType useage) { data.usageType = useage; };
		Data::UsageType GetUsageType() const { return data.usageType; };

		void Bind();
		void UnBind();


	public: // BVH
		struct BVHTriangle
		{
			graphics::Vertex VertexA;
			graphics::Vertex VertexB;
			graphics::Vertex VertexC;

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

			void GrowToInclude(BVHTriangle triangle)
			{
				GrowToInclude(triangle.VertexA.position);
				GrowToInclude(triangle.VertexB.position);
				GrowToInclude(triangle.VertexC.position);
			}
		};

		struct Node
		{
			BoundingBox BVH{};
			std::vector<BVHTriangle> Triangles{};
			Node* ChildA{};
			Node* ChildB{};
		};

		void Input_BVH(std::vector<graphics::Vertex>& vertices, std::vector<unsigned int>& indices);

		void Split(Node* parent, int depth = 0);

		Node* Get_Root() { return Root; }

		// debug
		void DrawNodes(Node* node, int depth = 0);

		void FreeNodes(Node* node);

	private:
		GLuint VBO;
		GLuint EBO;


		Data data;

		// BVH
		Node* Root = new Node;

		// 누수 없애기용

		std::vector<Node*> AllNode{};


		//UINT MaxBVHDepth{10};

	};
}