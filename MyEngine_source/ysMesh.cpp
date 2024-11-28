#include "ysMesh.h"
#include <iostream>
#include <deque>

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
