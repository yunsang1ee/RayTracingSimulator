#include "ysInputManager.h"
#include "YSapplication.h"


//extern ys::Application app;

namespace ys
{
	constexpr WORD kKeyDataSize = 256;//Alt조합 상정
	//Ctrl과 Ctrl + Alt까지 상정하면 2048까지
	std::vector<KeyState> InputManager::keys;
	math::Vector2 InputManager::mousePosition;

	void InputManager::Init()
	{
		keys.assign(kKeyDataSize, 0x00);
	}

	void InputManager::BeforeUpdate()
	{//이후 KF_ALTDOWN을 이용한 bool을 parameter로 받아서 Alt키 조합까지 구현가능
		// &= ~
		for (WORD keyIndex = 0; keyIndex < kKeyDataSize; ++keyIndex)
		{
			if (getKeyDown(keyIndex))
				keys[keyIndex] |= kKeyPressedBit;
			if (getKeyUp(keyIndex)) {
				keys[keyIndex] &= (~kKeyPressedBit);
			}
		}
	}

	void InputManager::AfterUpdate()
	{
		for (WORD keyIndex = 0; keyIndex < kKeyDataSize; ++keyIndex)
		{
			if (getKeyDown(keyIndex))
				keys[keyIndex] &= (~kKeyDownBit);
			if (getKeyUp(keyIndex))
			{
				keys[keyIndex] &= (~kKeyPressedBit);
				keys[keyIndex] &= (~kKeyUpBit);
			}
		}
	}

	void InputManager::setKeyState(BYTE code, bool isPressed, bool isUp)
	{
		if (isUp)
			keys[code] |= kKeyUpBit;  //isUp이면 up
		else if (isPressed)
			keys[code] |= kKeyPressedBit;  //Pressed면 pressed
		else
			keys[code] |= kKeyDownBit;     // 둘 다 아니면 down
	}
}