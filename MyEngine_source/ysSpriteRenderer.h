#pragma once
#include "ysComponent.h"
#include "ysTexture.h"
#include "ysShader.h"
#include <ysMesh.h>

namespace ys
{
	class SpriteRenderer : public Component
	{
		struct Material
		{
			glm::vec4 color;
			glm::vec4 emittedColor;
			float emissionStrength;
		};

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

		graphics::Texture* GetTexture() { return texture; }
		Mesh* GetMesh() { return mesh; }

		void SetTexture(graphics::Texture* texture) { this->texture = texture; }
		void SetShader(graphics::Shader* shader) { this->shader = shader; }
		void SetMesh(Mesh* mesh) { this->mesh = mesh; }
		void AddLight(GameObject* light, glm::vec3 color) { lights.emplace_back(light, color); }
		void DelLight(GameObject* light)
		{
			lights.erase(remove_if(lights.begin(), lights.end()
				, [&](const std::pair<GameObject*, glm::vec3>& val) {return val.first == light; })
				, lights.end());
		}
		void SetLightColor(GameObject* light, glm::vec3 color)
		{
			std::find_if(lights.begin(), lights.end(), [&](std::pair<GameObject*, glm::vec3>& val) {
				return val.first == light;
				}
			)->second = color;
		}
		void SetObjectColor(glm::vec4 color)
		{
			objectColor = color;
		}
		glm::vec3 GetLightColor(GameObject* light) const
		{
			return std::find_if(lights.begin(), lights.end(), [&](const std::pair<GameObject*, glm::vec3>& val) {
				return val.first == light;
				}
			)->second;
		}
		//void SetOffset(const glm::vec2& LT, const glm::vec2& RB = glm::vec2())
		//{
		//	this->offsetLT = LT;
		//	this->offsetRB = RB;
		//}
		//void SetSizeByTexture(const glm::vec2& size) { this->size = size; }
		//void SetSizeByScreen(const glm::vec2& size)
		//{
		//	this->size.x = size.x / texture->GetWidth();
		//	this->size.y = size.y / texture->GetHeight();
		//}

	private:
		graphics::Texture* texture;
		graphics::Shader* shader;
		Mesh* mesh;
		std::vector<std::pair<GameObject*, glm::vec3>> lights;
		glm::vec4 objectColor;
		bool isAmbient;
	};
}