#include "ysInputManager.h"
#include "YSapplication.h"


//extern ys::Application app;

namespace ys
{
	constexpr WORD kKeyDataSize = 256;//Alt���� ����
	//Ctrl�� Ctrl + Alt���� �����ϸ� 2048����
	std::vector<KeyState> InputManager::keys;
	math::Vector2 InputManager::mousePosition;

	void InputManager::Init()
	{
		keys.assign(kKeyDataSize, 0x00);
	}

	void InputManager::BeforeUpdate()
	{//���� KF_ALTDOWN�� �̿��� bool�� parameter�� �޾Ƽ� AltŰ ���ձ��� ��������
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
			keys[code] |= kKeyUpBit;  //isUp�̸� up
		else if (isPressed)
			keys[code] |= kKeyPressedBit;  //Pressed�� pressed
		else
			keys[code] |= kKeyDownBit;     // �� �� �ƴϸ� down
	}
}