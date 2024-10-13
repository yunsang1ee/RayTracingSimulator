#include "ysBoxCollider2D.h"
#include "ysGameObject.h"
#include <ysTransform.h>
#include <ysRenderer.h>

namespace ys
{
	using namespace math;

	BoxCollider2D::BoxCollider2D() : Collider(enums::ColliderType::Box2D)
	{
	}
	BoxCollider2D::~BoxCollider2D()
	{
	}
	void BoxCollider2D::Init()
	{
	}
	void BoxCollider2D::Update()
	{
	}
	void BoxCollider2D::LateUpdate()
	{
	}
	void BoxCollider2D::Render(HDC hDC)
	{
		if (isRender())
		{
			auto tr = GetOwner()->GetComponent<Transform>();
			auto position = tr->GetPosition();
			if (renderer::mainCamera)
				position = renderer::mainCamera->CalculatPosition(position);
			Vector2 scale = tr->GetScale();

			auto offset = GetOffset();
			auto brush = (HBRUSH)GetStockObject(NULL_BRUSH);
			auto oldBrush = SelectObject(hDC, brush);
			auto pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			auto oldPen = SelectObject(hDC, pen);
			Rectangle(hDC
				, position.x + offset.x
				, position.y + offset.y
				, position.x + offset.x + 100 * GetSize().x
				, position.y + offset.y + 100 * GetSize().y);
			SelectObject(hDC, oldPen);
			DeleteObject(pen);
			SelectObject(hDC, oldBrush);
		}
	}
}