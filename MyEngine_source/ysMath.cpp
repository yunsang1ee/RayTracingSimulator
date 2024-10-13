#include "ysMath.h"

namespace ys::math
{
	Vector2 Vector2::One = Vector2(1.0f, 1.0f);
	Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	Vector2 Vector2::Left = Vector2(-1.0f, 0.0f);
	Vector2 Vector2::Up = Vector2(0.0f, -1.0f);
	Vector2 Vector2::Down = Vector2(0.0f, 1.0f);

	double safeAcos(double value)
	{
		if (value <= -1.0) 
			return ys::math::kPi;
		else if (value >= 1.0) 
			return 0;
		else 
			return acos(value);
	}
	float safeAcosf(float value)
	{
		if (value <= -1.0) 
			return ys::math::kPi;
		else if (value >= 1.0) 
			return 0;
		else 
			return acos(value);
	}
}