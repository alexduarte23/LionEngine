#pragma once

#include <GLFW/glfw3.h>

namespace avt {
	struct Vector2;
	class Input;

	class App {
	public:
		App() {}
		virtual ~App () {}

		virtual void initCallback(GLFWwindow* win) {}
		virtual void displayCallback(GLFWwindow* win, float dt) {}
		virtual void updateCallback(GLFWwindow* win, float dt) {}
		virtual void pollEventsCallback(GLFWwindow* win, const Input& input, float dt) {}
		virtual void windowResizeCallback(GLFWwindow* win, int w, int h) {}
		virtual void windowCloseCallback(GLFWwindow* win) {}
		virtual void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {}
		virtual void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {}

	};
}