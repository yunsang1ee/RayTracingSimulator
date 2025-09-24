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

	struct PackedVertex
	{
		glm::vec4 positionU;
		glm::vec4 normalV;
	};

	struct Material
	{
		glm::vec4 color;
		glm::vec4 emittedColor;
		float emissionStrength;
		float smoothness;
		float specularProbability;
		int flag;

		Material() = default;
		Material(const glm::vec4& color, const glm::vec4& emittedColor
			, float emissionStrength, float smoothness, float specularProbability, int flag)
			: color(color), emittedColor(emittedColor)
			, emissionStrength{ emissionStrength }, smoothness(smoothness)
			, specularProbability(specularProbability), flag(flag)
		{
		}
		Material(const Material& other)
			: color(other.color), emittedColor(other.emittedColor)
			, emissionStrength(other.emissionStrength), smoothness(other.smoothness)
			, specularProbability(other.specularProbability), flag(other.flag)
		{
		}
	};

	struct TopLevelAccelerationStructure
	{
		glm::vec3 aabbMin;			// 12
		uint32_t beginNodeOffset;	// 4
		glm::vec3 aabbMax;			// 12
		uint32_t beginTriangleOffset;// 4
		glm::mat4 transform;		// 64
		glm::mat4 invTransform;		// 64
		Material material;			// 48
	};// 208 bytes

	struct GPUBVHNode
	{
		glm::vec3 minBounds;		// 12 bytes
		uint32_t leftChild;			// 4 bytes
		glm::vec3 maxBounds;		// 12 bytes  
		uint32_t triangleIndex;		// 4 bytes (leaf일 때)
		uint32_t triangleCount;		// 4 bytes (leaf일 때)
		uint32_t pad[3];			// 12 bytes
	}; // 48 bytes

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