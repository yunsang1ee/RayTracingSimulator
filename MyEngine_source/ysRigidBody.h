#pragma once
#include <ysComponent.h>

namespace ys
{
	class RigidBody : public Component
	{
	public:
		RigidBody();
		~RigidBody();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC) override;

		void SetGravity(const glm::vec2& gravity)
		{ this->gravity = gravity * 6.80665f * 100.0f; }
		void SetMass(const float& mass) { this->mass = mass; }
		void SetFriction(const float& friction) { this->friction = friction; }
		void SetGround(const bool& ground) { this->ground = ground; }
		void SetVelocity(const glm::vec2 velocity) { this->velocity = velocity; }
		void AddForce(const glm::vec2 force) { this->force += force; }

		glm::vec2 GetVelocity() const { return velocity; }
		bool IsGround() const { return ground; }

	private:
		bool ground;
		float mass;
		float friction;

		glm::vec2 force;
		glm::vec2 accelation;
		glm::vec2 limitVelocity;
		glm::vec2 velocity;
		glm::vec2 gravity;
	};
}

