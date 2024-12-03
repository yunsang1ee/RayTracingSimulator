#include "CameraScript.h"
#include "CameraScript.h"
#include <ysTransform.h>
#include <ysObject.h>
#include <ysInputManager.h>
#include <ysTimer.h>
#include <YSapplication.h>

extern ys::Application app;

CameraScript::CameraScript()
	: sensitive(0.5f)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::Init()
{
}

void CameraScript::Update()
{
	using namespace ys;
	if (InputManager::getKeyDown((WORD)Key::LEFT_BUTTON))
		mouseCenter = InputManager::getMousePosition();

	Transform* tr = GetOwner()->GetComponent<Transform>();
	if (InputManager::getKey((WORD)Key::LEFT_BUTTON))
	{
		auto rotation = tr->GetRotation();
		auto deltaPos = InputManager::getMousePosition() - mouseCenter;
		mouseCenter = InputManager::getMousePosition();
		rotation.y = std::fmodf((rotation.y - deltaPos.x), 360.0f);
		rotation.x = std::fmodf((rotation.x - deltaPos.y), 360.0f);
		tr->SetRotation(rotation);
	}
	glm::vec3 pos = tr->GetPosition();
	if (InputManager::getKey('d')) pos += glm::normalize(glm::vec3(tr->GetRight().x, 0.0f, tr->GetRight().z)) * (Timer::getDeltaTime() / sensitive);
	if (InputManager::getKey('a')) pos -= glm::normalize(glm::vec3(tr->GetRight().x, 0.0f, tr->GetRight().z)) * (Timer::getDeltaTime() / sensitive);
	if (InputManager::getKey('w')) pos += glm::normalize(glm::vec3(tr->GetForward().x, 0.0f, tr->GetForward().z)) * (Timer::getDeltaTime() / sensitive);
	if (InputManager::getKey('s')) pos -= glm::normalize(glm::vec3(tr->GetForward().x, 0.0f, tr->GetForward().z)) * (Timer::getDeltaTime() / sensitive);
	if (InputManager::getKey(GLFW_KEY_LEFT_SHIFT)) pos -= glm::vec3(0.0f, 1.0f, 0.0f) * (Timer::getDeltaTime() / sensitive);
	if (InputManager::getKey(GLFW_KEY_SPACE)) pos += glm::vec3(0.0f, 1.0f, 0.0f) * (Timer::getDeltaTime() / sensitive);
	tr->SetPosition(pos);

}

void CameraScript::LateUpdate()
{
}

void CameraScript::Render(HDC hDC)
{
}
