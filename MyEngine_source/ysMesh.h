#pragma once
#include "ysResource.h"
#include "ysGraphics.h"

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

	private:
		GLuint VBO;
		GLuint EBO;

		Data data;
	};
}