#pragma once
#include <ysComponent.h>
#include <ysAnimation.h>

namespace ys
{
	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				eventFunc = std::move(func);
			}
			void operator()()
			{
				if (eventFunc)
					eventFunc();
			}
			std::function<void()> eventFunc;
		};

		struct Events
		{
			Event startEvent;
			Event completeEvent;
			Event endEvent;
		};

		Animator();
		~Animator();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(HDC hDC) override;

		void CrateAnimation(const std::wstring& name, graphics::Texture* spriteSheet
			, const math::Vector2& leftTop, const math::Vector2& size, const math::Vector2& offset
			, const UINT lenth, const float duration, const bool& isReverse = false);

		Animation* FindAnimation(const std::wstring& name);
		Animation* GetActive() { return activeAnimation; }
		void PlayAnimation(const std::wstring& name, const bool loop = true);

		Events* FindEvents(const std::wstring& name);
		std::function<void()>& GetStartEvent(const std::wstring name);
		std::function<void()>& GetCompleteEvent(const std::wstring name);
		std::function<void()>& GetEndEvent(const std::wstring name);

		bool IsComplete() const { return activeAnimation->IsComplete(); }
	private:
		std::map<std::wstring, Animation*> animations;
		std::map<std::wstring, Events*> animationEvents;
		Animation* activeAnimation;
		bool isLoop;
	};

} 