#include "ysAnimator.h"
#include <cassert>

namespace ys
{
	Animator::Animator() : Component(ComponentType::Animator), animations{}, activeAnimation{ nullptr }, isLoop(false)
	{
	}
	Animator::~Animator()
	{
		for (auto& animation : animations)
		{
			delete animation.second;
			animation.second = nullptr;
		}
		for (auto& events : animationEvents)
		{
			delete events.second;
			events.second = nullptr;
		}
	}
	void Animator::Init()
	{
	}
	void Animator::Update()
	{
		if (activeAnimation)
		{
			activeAnimation->Upadate();

			Events* events = FindEvents(activeAnimation->getName());
			if (activeAnimation->IsComplete())
			{
				if(events->completeEvent.eventFunc != nullptr)
					events->completeEvent();

				if(isLoop)
					activeAnimation->Reset();
			}
		}
	}
	void Animator::LateUpdate()
	{
	}
	void Animator::Render(HDC hDC)
	{
		if (activeAnimation)
			activeAnimation->Render(hDC);
	}
	void Animator::CrateAnimation(const std::wstring& name, graphics::Texture* spriteSheet
		, const math::Vector2& leftTop, const math::Vector2& size, const math::Vector2& offset
		, const UINT lenth, const float duration, const bool& isReverse)
	{
		Animation* animation{ nullptr };
		animation = FindAnimation(name);
		if (animation != nullptr)
			return;

		animation = new Animation();
		animation->CrateAnimation(name, spriteSheet
			, leftTop, size, offset, lenth, duration, isReverse);
		animation->SetAnimatior(this);

		Events* events = new Events();
		animationEvents.insert(std::make_pair(name, events));
		animations.insert(std::make_pair(name, animation));
	}
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		auto iter = animations.find(name);
		if (iter == animations.end())
			return nullptr;

		return iter->second;
	}
	void Animator::PlayAnimation(const std::wstring& name, const bool loop)
	{
		Animation* animation = FindAnimation(name);
		if (animation == nullptr)
		{
			assert(false);
			return;
		}

		if(activeAnimation)
		{
			Events* curEvents = FindEvents(activeAnimation->getName());
			if (curEvents)
				curEvents->endEvent();
		}

		activeAnimation = animation;
		Events* nextEvents = FindEvents(activeAnimation->getName());
		if (nextEvents)
			nextEvents->startEvent();

		activeAnimation->Reset();
		isLoop = loop;
	}
	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		auto iter = animationEvents.find(name);
		if (iter == animationEvents.end())
			return nullptr;

		return iter->second;
	}
	std::function<void()>& Animator::GetStartEvent(const std::wstring name)
	{
		auto events = FindEvents(name);
		return events->startEvent.eventFunc;
	}
	std::function<void()>& Animator::GetCompleteEvent(const std::wstring name)
	{
		auto events = FindEvents(name);
		return events->completeEvent.eventFunc;
	}
	std::function<void()>& Animator::GetEndEvent(const std::wstring name)
	{
		auto events = FindEvents(name);
		return events->endEvent.eventFunc;
	}
}