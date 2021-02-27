#include "../HeaderFiles/Input.h"

namespace avt {

	GLFWwindow* Input::_win = nullptr;
	Vector2 Input::_scrollOffset;
	Vector2 Input::_mouseOffset;
	CursorMode Input::_cmode = CursorMode::Normal;

	std::unordered_map<KeyCode, int> Input::_keyStates;
	std::unordered_map<MouseCode, int> Input::_mouseStates;

	//void Input::setWindow(GLFWwindow* win) {
	//	Input::_win = win;
	//}

}