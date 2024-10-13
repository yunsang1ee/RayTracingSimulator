#include "ysCircleCollider2D.h"
#include "ysGameObject.h"
#include <ysTransform.h>
#include <ysRenderer.h>

namespace ys
{
	CircleCollider2D::CircleCollider2D() : Collider(enums::ColliderType::Circle2D)
	{
	}
	CircleCollider2D::~CircleCollider2D()
	{
	}
	void CircleCollider2D::Init()
	{
	}
	void CircleCollider2D::Update()
	{
		//if (sqrt(pow((point.x - center.x), 2) + pow((point.y - center.y), 2)) <= radius)
	}
	void CircleCollider2D::LateUpdate()
	{
	}
	void CircleCollider2D::Render(HDC hDC)
	{
		if (isRender())
		{
			auto tr = GetOwner()->GetComponent<Transform>();
			auto positionedByCamera = renderer::mainCamera->CalculatPosition(tr->GetPosition());
			auto offset = GetOffset();
			auto brush = (HBRUSH)GetStockObject(NULL_BRUSH);
			auto oldBrush = SelectObject(hDC, brush);
			auto pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			auto oldPen = SelectObject(hDC, pen);
			Ellipse(hDC, positionedByCamera.x + offset.x
				, positionedByCamera.y + offset.y
				, positionedByCamera.x + offset.x + 100 * GetSize().x
				, positionedByCamera.y + offset.y + 100 * GetSize().y);
			SelectObject(hDC, oldPen);
			DeleteObject(pen);
			SelectObject(hDC, oldBrush);
		}
	}
}