#pragma once
#include "ysComponent.h"
namespace ys
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hDC) override;

		void SetPosition(const glm::vec3& pos) { position = pos; }
		void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; }
		void SetScale(const glm::vec3& scale) { this->scale = scale; }

		glm::mat4 GetWorldMatrix() const { return worldMatrix; }
		void SetWorldMatrix(const glm::mat4& matrix) { worldMatrix = matrix; }

		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetScale() const { return scale; }
		glm::vec3 GetRotation() const { return rotation; }

		inline glm::vec3 GetForward() const { return forward; }
		inline glm::vec3 GetRight() const { return right; }
		inline glm::vec3 GetUp() const { return up; }
		
	private:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		glm::mat4 worldMatrix;

		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};

}