#pragma once
#include <CommonInclude.h>
#include <fstream>
#include <sstream>

namespace ys::graphics
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texture;
		glm::vec3 normal;

		Vertex(const glm::vec3& p, const glm::vec2& t, const glm::vec3& n)
			: position(p), texture(t), normal(n) {}
		Vertex()
			: position(), texture(), normal() {}

		bool operator==(const Vertex& other) const { return position == other.position && texture == other.texture && normal == other.normal; }
		bool operator<(const Vertex that) const { return memcmp((void*)this, (void*)&that, sizeof(Vertex)) > 0; }
	};
}

namespace std
{
	template <>
	struct hash<ys::graphics::Vertex>
	{
		std::size_t operator()(const ys::graphics::Vertex& v) const
		{
			auto hash1_1 = std::hash<float>()(v.position.x);
			auto hash1_2 = std::hash<float>()(v.position.y);
			auto hash1_3 = std::hash<float>()(v.position.z);
			auto hash2_1 = std::hash<float>()(v.texture.s);
			auto hash2_2 = std::hash<float>()(v.texture.t);
			auto hash3_1 = std::hash<float>()(v.normal.x);
			auto hash3_2 = std::hash<float>()(v.normal.y);
			auto hash3_3 = std::hash<float>()(v.normal.z);
			return (hash1_1 ^ (hash1_2 << 1) ^ (hash1_3 << 2))
				^ (hash2_1 ^ (hash2_2 << 1))
				^ (hash3_1 ^ (hash3_2 << 1) ^ (hash3_3 << 2));
		}
	};
}