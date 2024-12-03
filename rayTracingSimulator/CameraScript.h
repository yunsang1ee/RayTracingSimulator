#pragma once
#include "ysScript.h"
class CameraScript : public ys::Script
{
public:
	CameraScript();
	~CameraScript();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;

private:
	float sensitive;

	glm::vec2 mouseCenter;
	glm::vec2 mouseCurrent;
};

