#pragma once
#include "CommonInclude.h"

//#include <Windows.h>
//#include <vector>
//#include <string>
//#include <map>
//#include <utility>

namespace ys
{
	enum class Key : BYTE
	{
		//Num0 = '0', Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		//A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		//Colons = 0xBA,
		//ESC = 256, SPACE = 32,
		LEFT_BUTTON = 0x80, RIGHT_BUTTON, MIDDLE_BUTTON,
		//SHIFT = 0x80 + 3, CTRL, ALT,
		//F1 = 0x80 + 6, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		//LEFT = 0x80 + 6 + 0x64, UP, RIGHT, DOWN,
		//PAGE_UP, PAGE_DOWN, HOME, END, INSERT,
	} ; // OpenGL not Use

	using KeyState = BYTE;
	constexpr KeyState kKeyDownBit = 0b0001;
	constexpr KeyState kKeyPressedBit = 0b0010;
	constexpr KeyState kKeyUpBit = 0b0100;

	class InputManager
	{
	public:
		
		static void Init();//메세지 루프 이전에 필수사용
		static void BeforeUpdate();//메인 업데이트 이전 키 업데이트
		static void AfterUpdate();//메인 업데이트 이후 키 업데이트
		
		static void setKeyState(BYTE code, bool isPressed, bool isUp);
		static void setMousePosition(int x, int y) { mousePosition = {(float)x, (float)y}; }
		static math::Vector2 getMousePosition() { return mousePosition; }

		inline static bool getKeyDown(BYTE code) { return  (keys[static_cast<UINT>(code)] & kKeyDownBit) != 0; }
		inline static bool getKey(BYTE code) { return  (keys[static_cast<UINT>(code)] & kKeyPressedBit) != 0; }
		inline static bool getKeyUp(BYTE code) { return  (keys[static_cast<UINT>(code)] & kKeyUpBit); }

		inline static KeyState getKeyState(BYTE code) { return  keys[static_cast<UINT>(code)]; }
	private:
		static std::vector<KeyState> keys;
		static math::Vector2 mousePosition;
	};
}