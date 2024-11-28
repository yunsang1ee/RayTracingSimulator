#pragma once
#include <ysComponent.h>
#include <ysGameObject.h>

namespace ys
{
	class Camera : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		static glm::mat4 GetmainViewMatrix() { return mainViewMatrix; }
		static glm::mat4 GetmainProjectionMatrix() { return mainProjectionMatrix; }
		static void SetGpuViewMatrix(glm::mat4 matrix) { mainViewMatrix = matrix; }
		static void SetGpuProjectionMatrix(glm::mat4 matrix) { mainProjectionMatrix = matrix; }

		Camera();
		~Camera();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC) override;

		void SetNearFar(const float& nearZ, const float& farZ)
		{
			this->nearZ = nearZ;
			this->farZ = farZ;
		}

		void CreateViewMatrix();
		void CreateProjectionMatrix(ProjectionType);

	private:
		static glm::mat4 mainViewMatrix;
		static glm::mat4 mainProjectionMatrix;

		ProjectionType projectionType;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		float aspectRatio;
		float nearZ;
		float farZ;
		float size;
	};
}

