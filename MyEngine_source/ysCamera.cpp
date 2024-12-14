#include "ysCamera.h"
#include "ysTransform.h"
#include "YSapplication.h"
#include "ysInputManager.h"
#include "ysTimer.h"
#include "ysImgui_Manager.h"
#undef max
#undef min

extern ys::Application app;

namespace ys
{
	glm::mat4 Camera::mainProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 Camera::mainViewMatrix = glm::mat4(1.0f);

	Camera::Camera() : Component(enums::ComponentType::Camera)
		, projectionType(ProjectionType::Perspective)
		, viewMatrix(glm::mat4(1.0f))
		, projectionMatrix(1.0f)
		, nearZ(0.01f)
		, farZ(1000.0f)
		, size(1.0f)
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Init()
	{
	}
	void Camera::Update()
	{
	}
	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(projectionType);

		mainViewMatrix = viewMatrix;
		mainProjectionMatrix = projectionMatrix;
	}
	void Camera::Render(HDC hDC)
	{
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		const glm::vec3 pos = tr->GetPosition();
		const glm::vec3 forward = tr->GetForward();
		const glm::vec3 up = tr->GetUp();
		viewMatrix = glm::lookAt(pos, pos + forward, up);
	}

	void Camera::CreateProjectionMatrix(ProjectionType type)
	{
		aspectRatio = app.getScreenf().x / app.getScreenf().y;
		switch (type)
		{
		case ProjectionType::Perspective:
			projectionMatrix = glm::perspective(Imgui_Manager::Get_Imgui_Manager()->Get_Fov(), aspectRatio, nearZ, farZ);
			break;
		case ProjectionType::Orthographic:
			auto width = app.getScreenf().x / size;
			auto height = app.getScreenf().y / size;
			projectionMatrix = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, nearZ, farZ);
			break;
		}
	}
}