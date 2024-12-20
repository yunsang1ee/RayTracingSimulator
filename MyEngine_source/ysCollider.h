#pragma once
#include "ysComponent.h"
#include "ysScript.h"

namespace ys
{
	class Collider : public Component
	{
	public:
		Collider(enums::ColliderType type);
		~Collider();

		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hDC);

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

		void SetOffset(const glm::vec2& offset) { this->offset = offset; }
		void SetSize(const glm::vec2& size) { this->size = size; }
		glm::vec2 GetOffset() { return offset; }
		glm::vec2 GetSize() { return size; }

		static void SetRender(const bool& activeRender_) { activeRender = activeRender_; }
		static bool isRender() { return activeRender; }

		UINT32 GetID() const { return id; }
		enums::ColliderType GetColliderType() const { return type; }

	private:
		enums::ColliderType type;
		static bool activeRender;
		static UINT32 collisionID; // collisionID 현황을 알기 위한 정적변수
		UINT32 id; // 인스턴스의 ID
		glm::vec2 offset;
		glm::vec2 size;
	};
}

