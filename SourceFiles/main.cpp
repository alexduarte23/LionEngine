#include <iostream>
#include <vector>
#include <memory>


#include "../HeaderFiles/Engine.h"



class MyApp : public avt::App {
private:
	avt::Shader _shader, _shaderFire, _shaderHUD, _shaderBG;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene;
	std::unique_ptr<avt::Camera> _cam;

	bool _cursorVisible = false;

	avt::SceneNode* _crosshair = nullptr;
	std::string _activeCam = "per";

	double _time = 0;

	void createScene() {
		avt::StencilPicker::enable();

		// MESHES
		//auto cubeM = _meshes.add("cube", new avt::Mesh("./Resources/Objects/cube_vtn_flat.obj"));
		//cubeM->setup();
		//
		//auto colorCubeM = _meshes.add("colorCube", new avt::Mesh("./Resources/Objects/colourscube.obj"));
		//colorCubeM->setup();

		auto cubeM = new avt::Mesh("./Resources/Objects/colourscube.obj");
		cubeM->setup();
		cubeM->setShader(&_shader);
		_scene.addNode(cubeM);
	}

		

	void processKeyInput(GLFWwindow* win, double dt) {
		avt::Vector3 move;
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) move.z += 1;
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) move.z -= 1;
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) move.x -= 1;
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) move.x += 1;
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) move.y += 1;
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) move.y -= 1;

		_cam->processMove(move, (float)dt);
	}

	void processMouseMovement(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double  dt) {
		auto offset = newCursor - lastCursor;
		if (!_cursorVisible) { // free move
			_cam->processOrbit(offset, (float)dt, true);

		} else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { // drag move
			_cam->processOrbit(offset, (float)dt);

		}

	}

	void createShaders() {
		// create regular mesh shader
		avt::ShaderParams params;
		params.setVertexShader("./Resources/shaders/basic-vs.glsl")
			.setFragmentShader("./Resources/shaders/basic-fs.glsl")
			.addInputs({ "position", "texCoord", "normal", "color" })
			.addUniform("ModelMatrix")
			.addUniformBlock("CameraMatrices", 0);
		_shader.create(params);

	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		float aspect = winx / (float)winy;

		_cam.reset(new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(0, 5.f, 10.f)));
		_cam->setMoveSpeed(12.f);

		_ub.create(2 * (long long)avt::LayoutElement::getTypeSize(avt::ShaderDataType::MAT4), 0);
		_ub.unbind();
		_cam->linkUBO(&_ub);
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void initCallback(GLFWwindow* win) override {
		createCams(win);
		createShaders();
		createScene();
	}

	void pollEventsCallback(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double dt) override {
		processKeyInput(win, dt);
		processMouseMovement(win, lastCursor, newCursor, dt);
	}

	void updateCallback(GLFWwindow* win, double dt) override {
		_time += dt;
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		_renderer.clear();

		_renderer.draw(_scene, _cam.get());
		
		if (_cursorVisible) avt::StencilPicker::getTargetOnCursor(win);
		else				avt::StencilPicker::getTargetOnCenter(win);
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		_cam->resize(w, h);
	}

	void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override {
		if (action != GLFW_PRESS) return;

		switch (key) {
		case GLFW_KEY_ESCAPE:
			_cursorVisible = !_cursorVisible;
			if (_cursorVisible) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case GLFW_KEY_0:
			_cam->setPosition(avt::Vector3(5.f, 5.f, 5.f));
			_cam->lookAt({});
			break;
		}
	}

	void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			auto target = avt::StencilPicker::getLastPick();
			if (target.second == "fire") {
				//_fireEmitter->toggle();
			}

		}
	}

};


int main(int argc, char* argv[]) {
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	avt::App *app = new MyApp();
	avt::Engine engine;
	engine.setApp(app);
	engine.setOpenGL(gl_major, gl_minor);
	engine.setWindow(1280, 960, "Up High", is_fullscreen, is_vsync);

	engine.init();
	engine.run();
	delete app;
	engine.shutdown();
	
	//exit(EXIT_SUCCESS);
	return 0;
}