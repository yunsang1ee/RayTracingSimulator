#include "ysCamera.h"
#include "ysTransform.h"
#include "YSapplication.h"
#include "ysInputManager.h"
#include "ysTimer.h"
#undef max
#undef min

extern ys::Application app;

namespace ys
{
	Camera::Camera() : Component(enums::ComponentType::Camera)
		, distance(Vector2::Zero), resolution(app.getScreenf())
		, target(nullptr)
		, minPosition(Vector2(resolution.x / 2.0f, resolution.y / 2.0f))
		, maxPosition(Vector2(16000, resolution.y / 2.0f))
		, xMin(false), xMax(false), yMin(false), yMax(false)
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
		if (target)
		{
			auto tr = target->GetComponent<Transform>();
			lookPosition = tr->GetPosition();
		}
		else
		{
			auto tr = GetOwner()->GetComponent<Transform>();
			lookPosition = tr->GetPosition();
		}
		auto prevLookPosition = lookPosition;
		lookPosition.x = std::max(minPosition.x, std::min(maxPosition.x, lookPosition.x));
		lookPosition.y = std::max(minPosition.y, std::min(maxPosition.y, lookPosition.y));
		
		if (lookPosition.x == minPosition.x) xMin = true;
		if (lookPosition.x == maxPosition.x) xMax = true;
		if (lookPosition.x == prevLookPosition.x)
		{
			xMin = false;
			xMax = false;
		}
		if (lookPosition.y == minPosition.y) yMin = true;
		if (lookPosition.y == maxPosition.y) yMax = true;
		if (lookPosition.y == prevLookPosition.y)
		{
			yMin = false;
			yMax = false;
		}

		distance = lookPosition - (resolution / 2.0f);
	}
	void Camera::LateUpdate()
	{
	}
	void Camera::Render(HDC hDC)
	{
		auto tr = GetOwner()->GetComponent<Transform>();
		auto position = tr->GetPosition();
		/*TextOut(hDC, position.x, position.y,
			(L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y)).c_str()
			, (L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y)).size());*/
	}
}