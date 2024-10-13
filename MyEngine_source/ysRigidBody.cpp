#include "ysRigidBody.h"
#include "ysGameObject.h"
#include "ysTransform.h"
#include "ysTimer.h"

namespace ys
{
	using namespace math;
	RigidBody::RigidBody() : Component(enums::ComponentType::RigidBody)
		, ground(false)
		, mass(1.0f)
		, friction(100.0f)
		, force(Vector2::Zero)
		, accelation(Vector2::Zero)
		, limitVelocity(Vector2(500.0f, 1000.0f))
		, velocity(Vector2::Zero)
		, gravity(Vector2::Down * 6.80665f * 100.0f)
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

		Vector2 tickGravity = gravity;
		tickGravity.nomalize();
		float dot = Vector2::Dot(velocity, tickGravity);
		tickGravity = tickGravity * dot;

		if (ground)
			velocity = velocity - tickGravity;
		else
			velocity += gravity * Timer::getDeltaTime();

		Vector2 sideVelocity = velocity - tickGravity;
		if (limitVelocity.y < tickGravity.scalar())
			tickGravity = tickGravity.nomalize() * limitVelocity.y;

		if (limitVelocity.x < sideVelocity.scalar())
			sideVelocity = sideVelocity.nomalize() * limitVelocity.x;

		velocity = tickGravity + sideVelocity;

		if (velocity != Vector2::Zero)
		{
			Vector2 tickFriction = -velocity;
			tickFriction = tickFriction.nomalize() * friction * mass * Timer::getDeltaTime();
			if (velocity.scalar() <= tickFriction.scalar())
				velocity = Vector2::Zero;
			else
				velocity += tickFriction;
		}
		auto tr = GetOwner()->GetComponent<Transform>();
		Vector2 position = tr->GetPosition();
		position += velocity * Timer::getDeltaTime();
		tr->SetPosition(position);

		force = Vector2::Zero;
	}

	void RigidBody::LateUpdate()
	{
	}

	void RigidBody::Render(HDC hDC)
	{
	}
}