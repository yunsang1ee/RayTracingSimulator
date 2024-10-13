#include "ysTransform.h"
#include <ysInputManager.h>
#include <ysTimer.h>
#include <ysGameObject.h>
#include <ysSpriteRenderer.h>

namespace ys
{

	Transform::Transform()
		: Component(enums::ComponentType::Transform)
		, scale(Vector2::One)
		, rotation(0.0f)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Init()
	{
	}

	void Transform::Update()
	{
	}

	void Transform::LateUpdate()
	{
	}

	void Transform::Render(HDC hDC)
	{
	}
}
