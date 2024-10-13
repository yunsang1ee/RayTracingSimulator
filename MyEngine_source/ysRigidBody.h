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

		void SetGravity(const math::Vector2& gravity)
		{ this->gravity = gravity * 6.80665f * 100.0f; }
		void SetMass(const float& mass) { this->mass = mass; }
		void SetFriction(const float& friction) { this->friction = friction; }
		void SetGround(const bool& ground) { this->ground = ground; }
		void SetVelocity(const math::Vector2 velocity) { this->velocity = velocity; }
		void AddForce(const math::Vector2 force) { this->force += force; }

		math::Vector2 GetVelocity() const { return velocity; }
		bool IsGround() const { return ground; }

	private:
		bool ground;
		float mass;
		float friction;

		math::Vector2 force;
		math::Vector2 accelation;
		math::Vector2 limitVelocity;
		math::Vector2 velocity;
		math::Vector2 gravity;
	};
}

