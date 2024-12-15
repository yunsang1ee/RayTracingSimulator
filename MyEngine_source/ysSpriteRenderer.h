#pragma once
#include "ysComponent.h"
#include "ysTexture.h"
#include "ysShader.h"
#include <ysMesh.h>

namespace ys
{
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
			, emissionStrength{emissionStrength}, smoothness(smoothness)
			, specularProbability(specularProbability), flag(flag)
		{}
		Material(const Material& other)
			: color(other.color), emittedColor(other.emittedColor)
			, emissionStrength(other.emissionStrength), smoothness(other.smoothness)
			, specularProbability(other.specularProbability), flag(other.flag)
		{}
	};
	class SpriteRenderer : public Component
	{

	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hDC) override;

		//glm::vec2 GetSize() { return size; }
		//glm::vec2 GetOffsetLT() { return offsetLT; }
		//glm::vec2 GetOffsetRB() { return offsetRB; }

		graphics::Texture* GetTexture() { return texture;}
		Mesh* GetMesh() { return mesh;}
		Material GetMaterial() const { return material;}

		void SetTexture(graphics::Texture* texture) { this->texture = texture;}
		void SetShader(graphics::Shader* shader) { this->shader = shader;}
		void SetMesh(Mesh* mesh) { this->mesh = mesh;}
		void SetObjectColor(const glm::vec4& color) { material.color = color;}
		void SetLightColor(const glm::vec4& color) { material.emittedColor = color;}
		void SetLightStrength(const float& strength) { material.emissionStrength = strength; }
		void SetSmoothness(const float& smoothness) { material.smoothness = smoothness; }
		void SetSpecularProbability(const float& specularProbability) { material.specularProbability = specularProbability; }
		void IsSun() { material.flag = 1; }

	private:
		graphics::Texture* texture;
		graphics::Shader* shader;
		Mesh* mesh;
		Material material;
		bool isAmbient;
	};
}