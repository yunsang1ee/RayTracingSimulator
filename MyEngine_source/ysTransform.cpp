#include "ysTransform.h"
#include <ysInputManager.h>
#include <ysTimer.h>
#include <ysGameObject.h>
#include <ysSpriteRenderer.h>

namespace ys
{

	Transform::Transform()
		: Component(enums::ComponentType::Transform)
		, scale(glm::vec3(1.0f))
		, rotation(glm::vec3())
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
		glm::mat4 matScale = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 matRotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));
		matRotation = glm::rotate(matRotation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matRotation = glm::rotate(matRotation, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), position);

		worldMatrix = matTranslation * matRotation * matScale;

		forward =	glm::inverseTranspose(matRotation) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		right =		glm::inverseTranspose(matRotation) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		up =		glm::inverseTranspose(matRotation) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	void Transform::Render(HDC hDC)
	{
	}
}
