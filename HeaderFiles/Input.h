#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>
#include "avt_math.h"


namespace avt {

	enum class KeyCode {
		// From glfw3.h
		Unknown = -1,
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};

	enum class MouseCode {
		// From glfw3.h
		Unknown = -1,
		Btn0 = 0,
		Btn1 = 1,
		Btn2 = 2,
		Btn3 = 3,
		Btn4 = 4,
		Btn5 = 5,
		Btn6 = 6,
		Btn7 = 7,

		BtnLast = Btn7,
		BtnLeft = Btn0,
		BtnRight = Btn1,
		BtnMiddle = Btn2
	};

	class Input {
	private:
		GLFWwindow* _win = nullptr;
		Vector2 _scrollOffset;
		Vector2 _mouseOffset;
	
		std::unordered_map<KeyCode, int> _keyStates;
		std::unordered_map<MouseCode, int> _mouseStates;

		friend class Engine;

	public:
		Input() {}
		~Input() {}

		Input(const Input& input) = delete;
		Input(Input&& input) = delete;
		Input& operator=(const Input& input) = delete;
		Input& operator=(Input&& input) = delete;

		bool keyPressed(KeyCode key) const {
			auto state = _keyStates.find(key);
			if (state == _keyStates.end()) return false;
			else return state->second == GLFW_PRESS;
		}

		bool keyDown(KeyCode key) const {
			if (!_win) return false;
			return glfwGetKey(_win, static_cast<std::underlying_type<KeyCode>::type>(key)) == GLFW_PRESS;
		}

		bool keyReleased(KeyCode key) const {
			auto state = _keyStates.find(key);
			if (state == _keyStates.end()) return false;
			else return state->second == GLFW_RELEASE;
		}

		bool mouseBtnPressed(MouseCode btn) const {
			auto state = _mouseStates.find(btn);
			if (state == _mouseStates.end()) return false;
			else return state->second == GLFW_PRESS;
		}

		bool mouseBtnDown(MouseCode btn) const {
			if (!_win) return false;
			return glfwGetMouseButton(_win, static_cast<std::underlying_type<MouseCode>::type>(btn)) == GLFW_PRESS;
		}

		bool mouseBtnReleased(MouseCode btn) const {
			auto state = _mouseStates.find(btn);
			if (state == _mouseStates.end()) return false;
			else return state->second == GLFW_RELEASE;
		}

		Vector2 mousePosition() const {
			if (_win) {
				double xcursor, ycursor;
				glfwGetCursorPos(_win, &xcursor, &ycursor);
				return { (float)xcursor, (float)ycursor };
			}
			return {};
		}

		Vector2 mouseOffset() const {
			return _mouseOffset;
		}

		// TODO
		Vector2 scroll() const {
			return _scrollOffset;
		}

		float scrollY() const {
			return _scrollOffset.y;
		}

		float scrollX() const {
			return _scrollOffset.x;
		}

	};

}