#pragma once
#include <ysComponent.h>
#include <ysGameObject.h>

namespace ys
{
	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC) override;

		void SetMinMax(const Vector2& minPosition, const Vector2& maxPosition) 
		{
			this->minPosition = minPosition;
			this->maxPosition = maxPosition;
		}
		Vector2 CalculatPosition(const Vector2& position) { return position - distance; }
		Vector2 GetLookPosition() const { return lookPosition; }
		void SetTarget(GameObject* target) { this->target = target; }
		bool isXmin() const { return xMin; }
		bool isXmax() const { return xMax; }
		bool isYmin() const { return yMin; }
		bool isYmax() const { return yMax; }

	private:
		Vector2 distance;
		Vector2 resolution;
		Vector2 lookPosition;//카메라의 중앙 (rect left, top) + (resolution / 2)

		Vector2 minPosition;//카메라 좌상단 제한
		Vector2 maxPosition;//카메라 우하단 제한

		bool xMin;
		bool xMax;
		bool yMin;
		bool yMax;

		GameObject* target;
	};
}

