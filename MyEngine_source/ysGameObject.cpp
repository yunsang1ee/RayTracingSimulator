#include "ysGameObject.h"
#include "ysSceneManager.h"
#include "ysInputManager.h"
#include "ysTimer.h"
#include <iostream>
#include <random>
#include <ysTransform.h>
#include <ysObject.h>

ys::GameObject::GameObject() : state(State::Active), ownLayer(LayerType::None)
{
	components.resize(static_cast<UINT>(enums::ComponentType::End));
	AddComponent<Transform>();
}

ys::GameObject::~GameObject()
{
	for (auto component : components)
	{
		delete component;
		component = nullptr;
	}
}

void ys::GameObject::Init()
{
	for (auto component : components)
	{
		if (component == nullptr) continue;
		component->Init();
	}
}

void ys::GameObject::Update()
{
	for (auto component : components)
	{
		if (component == nullptr) continue;
		component->Update();
	}
}

void ys::GameObject::LateUpdate()
{
	for (auto component : components)
	{
		if (component == nullptr) continue;
		component->LateUpdate();
	}
}

void ys::GameObject::Render(HDC hDC)
{
	for (auto component : components)
	{
		if (component == nullptr) continue;
		component->Render(hDC);
	}
	
}