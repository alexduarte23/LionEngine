#include "../HeaderFiles/Engine.h"
#include <vector>

namespace avt {

	////////////////////////////////////////////////////////////////////////////////// CALLBACKS


	void Engine::window_close_callback(GLFWwindow* win) {
		Engine* engine = (Engine*)glfwGetWindowUserPointer(win);
		engine->app()->windowCloseCallback(win);
	}

	void Engine::window_size_callback(GLFWwindow* win, int winx, int winy) {
		Engine* engine = (Engine*)glfwGetWindowUserPointer(win);
		engine->app()->windowResizeCallback(win, winx, winy);
	}

	void Engine::glfw_error_callback(int error, const char* description) {
		std::cerr << "GLFW Error: " << description << std::endl;
	}

	void Engine::window_key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
		Engine* engine = (Engine*)glfwGetWindowUserPointer(win);
		engine->app()->keyCallback(win, key, scancode, action, mods);
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
			Input::_keyStates[static_cast<KeyCode>(key)] = action;
	}

	void Engine::window_mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
		Engine* engine = (Engine*)glfwGetWindowUserPointer(win);
		engine->app()->mouseButtonCallback(win, button, action, mods);
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
			Input::_mouseStates[static_cast<MouseCode>(button)] = action;
	}


	////////////////////////////////////////////////////////////////////////////////// SETUP

	void Engine::init() {
		setupGLFW();
		setupGLEW();
		setupOpenGL();
#ifdef ERROR_CALLBACK
		_errorManager = ErrorManager(true);
		_errorManager.setupErrorCallback();
#else
		_errorManager = ErrorManager(false);
#endif
		_app->onInit(_win);
	}

	void Engine::setupGLFW() {
		glfwSetErrorCallback(Engine::glfw_error_callback);
		if (!glfwInit()) exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _glMinor);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		glfwWindowHint(GLFW_SAMPLES, 4);

		setupWindow();

		glfwSetWindowCloseCallback(_win, Engine::window_close_callback);
		glfwSetWindowSizeCallback(_win, Engine::window_size_callback);
		//glfwSetInputMode(_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetWindowUserPointer(_win, this);
		glfwSetKeyCallback(_win, Engine::window_key_callback);
		glfwSetMouseButtonCallback(_win, Engine::window_mouse_button_callback);

#if _DEBUG
		std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	}

	void Engine::setupWindow() {
		GLFWmonitor* monitor = _fullscreen ? glfwGetPrimaryMonitor() : 0;

		_win = glfwCreateWindow(_winX, _winY, _winTitle, monitor, 0);
		if (!_win) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(_win);
		glfwSwapInterval(_vsync);
		
		Input::_win = _win;
	}


	void Engine::setupGLEW() {
		glewExperimental = GL_TRUE;
		// Allow extension entry points to be loaded even if the extension isn't 
		// present in the driver's extensions string.
		GLenum result = glewInit();
		if (result != GLEW_OK) {
			std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
			exit(EXIT_FAILURE);
		}
		GLenum err_code = glGetError();
		// You might get GL_INVALID_ENUM when loading GLEW.
	}

	void Engine::setupOpenGL() {
#if _DEBUG
		checkOpenGLInfo();
#endif
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0, 1.0);
		glClearDepth(1.0);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_MULTISAMPLE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glViewport(0, 0, _winX, _winY);
	}

	void Engine::checkOpenGLInfo() {
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
		std::cerr << "OpenGL version " << version << std::endl;
		std::cerr << "GLSL version " << glslVersion << std::endl;
	}


	////////////////////////////////////////////////////////////////////////////////// ENGINE LOOP


	void Engine::run() {
		double xcursor = 0;
		double ycursor = 0;
		glfwGetCursorPos(_win, &xcursor, &ycursor);
		Vector2 lastCursor((float)xcursor, (float)ycursor);
		Vector2 newCursor;
		double last_time = glfwGetTime();

		while (!glfwWindowShouldClose(_win)) {
			double time = glfwGetTime();
			double elapsed_time = time - last_time;
			last_time = time;

			glfwPollEvents();
			glfwGetCursorPos(_win, &xcursor, &ycursor);
			Input::_mouseOffset = Vector2((float)xcursor, (float)ycursor) - lastCursor;

			_app->onUpdate(_win, (float)elapsed_time);
			Input::_keyStates.clear();
			Input::_mouseStates.clear();
			lastCursor.x = (float)xcursor;
			lastCursor.y = (float)ycursor;

			_app->onDisplay(_win, (float)elapsed_time);

			glfwSwapBuffers(_win);
		}
	}

	void Engine::shutdown() {
		glfwDestroyWindow(_win);
		glfwTerminate();
	}

}