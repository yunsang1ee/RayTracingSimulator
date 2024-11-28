#include "ysRigidBody.h"
#include "ysGameObject.h"
#include "ysTransform.h"
#include "ysTimer.h"

namespace ys
{
	RigidBody::RigidBody() : Component(enums::ComponentType::RigidBody)
		, ground(false)
		, mass(1.0f)
		, friction(100.0f)
		, force(glm::vec2())
		, accelation(glm::vec2())
		, limitVelocity(glm::vec2(500.0f, 1000.0f))
		, velocity(glm::vec2())
		, gravity(glm::vec2(0.0f, -1.0f) * 6.80665f * 100.0f)
	{
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::Init()
	{
	}

	void RigidBody::Update()
	{
		accelation = force / mass;// f = ma
		velocity += accelation * Timer::getDeltaTime();

		glm::vec2 tickGravity = glm::normalize(gravity);
		float dot = glm::dot(velocity, tickGravity);
		tickGravity = tickGravity * dot;

		if (ground)
			velocity = velocity - tickGravity;
		else
			velocity += gravity * Timer::getDeltaTime();

		glm::vec2 sideVelocity = velocity - tickGravity;
		if (limitVelocity.y < glm::length(tickGravity))
			tickGravity = glm::normalize(tickGravity) * limitVelocity.y;

		if (limitVelocity.x < glm::length(sideVelocity))
			sideVelocity = glm::normalize(sideVelocity) * limitVelocity.x;

		velocity = tickGravity + sideVelocity;

		if (velocity != glm::vec2())
		{
			glm::vec2 tickFriction = -velocity;
			tickFriction = glm::normalize(tickFriction) * friction * mass * Timer::getDeltaTime();
			if (glm::length(velocity) <= glm::length(tickFriction))
				velocity = glm::vec2();
			else
				velocity += tickFriction;
		}
		auto tr = GetOwner()->GetComponent<Transform>();
		glm::vec3 position = tr->GetPosition();
		position += glm::vec3(velocity * Timer::getDeltaTime(), 0);
		tr->SetPosition(position);

		force = glm::vec2();
	}

	void RigidBody::LateUpdate()
	{
	}

	void RigidBody::Render(HDC hDC)
	{
	}
}