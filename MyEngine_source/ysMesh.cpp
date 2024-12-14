#include "ysMesh.h"
#include <iostream>
#include <deque>
#include "ysImgui_Manager.h"

ys::Mesh::Data::Data()
{
}

ys::Mesh::Data::~Data()
{
}

ys::Mesh::Mesh()
	: Resource(enums::ResourceType::Mesh)
{
}

ys::Mesh::~Mesh()
{
	FreeNodes(Root);
}

HRESULT ys::Mesh::Load(const std::wstring& path)
{
	using namespace std;
	std::ifstream file(L"..\\Object\\" + path + L".obj");
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	string buff;
	vector<graphics::Vertex> vertices;
	vector<glm::vec3> positions;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;
	vector<unsigned int> indices;
	unordered_map<graphics::Vertex, unsigned int> vertexIndexMap;

	while (getline(ss, buff))
	{
		stringstream line(buff);
		string prefix;

		line >> prefix;
		if (prefix == "v")
		{
			glm::vec3 vertex;
			line >> vertex.x >> vertex.y >> vertex.z;
			positions.push_back(vertex);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texture{};
			line >> texture.x >> texture.y;
			textures.push_back(texture);
		}
		else if (prefix == "vn")
		{
			glm::vec3 normal{};
			line >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (prefix == "f")
		{
			std::string vertexData;
			while (line >> vertexData)
			{
				stringstream vertexStream(vertexData);
				std::string vertexStr, textureStr, normalStr;

				std::getline(vertexStream, vertexStr, '/');
				std::getline(vertexStream, textureStr, '/');
				std::getline(vertexStream, normalStr, '/');

				int vertexIndex = std::stoi(vertexStr);
				int textureIndex = textureStr.empty() ? -1 : std::stoi(textureStr);
				int normalIndex = normalStr.empty() ? -1 : std::stoi(normalStr);

				graphics::Vertex vertex{};
				vertex.position = positions[vertexIndex - 1];
				if (textureIndex != -1) vertex.texture = textures[textureIndex - 1];
				if (normalIndex != -1) vertex.normal = normals[normalIndex - 1];

				if (vertexIndexMap.find(vertex) == vertexIndexMap.end())
				{
					unsigned int newIndex = vertices.size();
					vertexIndexMap[vertex] = newIndex;
					vertices.push_back(vertex);
				}
				indices.push_back(vertexIndexMap[vertex]);
			}
		}
	}
	
	CreateVB(vertices);
	CreateEB(indices);

	// BVH 정렬
	Input_BVH(vertices, indices);

	return S_OK;
}

bool ys::Mesh::CreateVB(std::vector<graphics::Vertex>& vertices)
{
	data.vertices = vertices;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::Vertex) * vertices.size(), vertices.data(), /*GL_DYNAMIC_DRAW*//*GL_STREAM_DRAW*/GL_STATIC_DRAW);
	return true;
}

bool ys::Mesh::CreateEB(std::vector<GLuint>& indices)
{
	data.indices = indices;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	return true;
}

void ys::Mesh::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void ys::Mesh::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ys::Mesh::Input_BVH(std::vector<graphics::Vertex>& vertices, std::vector<unsigned int>& indices)
{

	BoundingBox BVH{};

	for (int i =0; i < vertices.size(); ++ i)
	{
		BVH.GrowToInclude(vertices[i].position);
	}

	std::vector<BVHTriangle> triangles{};

	for (int i = 0; i < indices.size(); ++i)
	{
		// 0
		graphics::Vertex a = vertices[indices[i++]];
		// 1
		graphics::Vertex b = vertices[indices[i++]];
		// 2
		graphics::Vertex c = vertices[indices[i]];

		glm::vec3 center = glm::vec3((a.position + b.position + c.position) / 3.f);

		triangles.push_back(BVHTriangle(a,b,c,center));
	}


	Root->Triangles = triangles;
	Root->BVH = BVH;

	Split(Root);
}

void ys::Mesh::Split(Node* parent, int depth)
{
	if (parent->Triangles.size() <= MAX_BVH_TRIANGLE_NUM)
	{
		return;
	}

	glm::vec3 Size = parent->BVH.Size;
	int splitAxis = Size.x > glm::max(Size.y, Size.z) ? 0 : Size.y > Size.z ? 1 : 2;

	float BVH_Center = parent->BVH.Center[splitAxis];



	for (int i = 0; i < parent->Triangles.size(); ++i)
	{

		// 부모의 바운딩볼륨 중심보다 작은지 판단?
		bool inA = parent->Triangles[i].Center[splitAxis] < BVH_Center;

		if (inA) // 부모의 바운딩볼륨 중심보다 작다면?
		{
			if (parent->ChildA == nullptr)
			{
				parent->ChildA = new Node{};
				AllNode.push_back(parent->ChildA);
			}

			// A 그룹으로 들어가기
			parent->ChildA->Triangles.push_back(parent->Triangles[i]);
			parent->ChildA->BVH.GrowToInclude(parent->Triangles[i]);
		}
		else
		{
			if (parent->ChildB == nullptr)
			{
				parent->ChildB = new Node{};
				AllNode.push_back(parent->ChildB);
			}

			// B 그룹으로 들어가기
			parent->ChildB->Triangles.push_back(parent->Triangles[i]);
			parent->ChildB->BVH.GrowToInclude(parent->Triangles[i]);
		}

	}

	// 우히히
	if (parent->ChildA != nullptr)
	{
		Split(parent->ChildA, depth + 1);
	}
	if (parent->ChildB != nullptr)
	{
		Split(parent->ChildB, depth + 1);
	}
}

void ys::Mesh::DrawNodes(Node* node, int depth)
{
	// 테스트용 렌더에서 호출함

	if (node == nullptr)
	{
		return;
	}

	std::cout << "depth" << depth << std::endl;

	std::cout << "triangle Num : " << node->Triangles.size() << std::endl;

	std::cout << "BV_size : " << node->BVH.Size.x << ", " << node->BVH.Size.y << ", " << node->BVH.Size.z << std::endl;

	

	DrawNodes(node->ChildA, depth + 1);

	DrawNodes(node->ChildB, depth + 1);

}


void ys::Mesh::FreeNodes(Node* node)
{
	if (node == nullptr)
	{
		return;
	} 
	FreeNodes(node->ChildA); 
	FreeNodes(node->ChildB); 
	delete node; // 동적으로 할당된 노드 메모리 해제
}
