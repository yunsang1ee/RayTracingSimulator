#include "ysComponent.h"

namespace ys
{
	Component::Component(ComponentType type)
		: myType(type), owner(nullptr)
	{
	}
	Component::~Component()
	{
	}
	void Component::Init()
	{
	}
	void Component::Update()
	{
	}
	void Component::LateUpdate()
	{
	}
	void Component::Render(HDC hDC)
	{
	}
}