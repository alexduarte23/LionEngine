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

	float _time = 0;

	void createScene() {
		avt::StencilPicker::enable();

		// cube_vtn_flat
		auto cubeM = new avt::Mesh("./Resources/Objects/colourscube.obj");
		cubeM->setup();
		cubeM->setShader(&_shader);
		_scene.addNode(cubeM);
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
		_cam->setOrbitSpeed(.45f);

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

	void pollEventsCallback(GLFWwindow* win, const avt::Input& input, float dt) override {
		avt::Vector3 move;
		if (input.keyDown(avt::KeyCode::W)) move.z += 1;
		if (input.keyDown(avt::KeyCode::S)) move.z -= 1;
		if (input.keyDown(avt::KeyCode::A)) move.x -= 1;
		if (input.keyDown(avt::KeyCode::D)) move.x += 1;
		if (input.keyDown(avt::KeyCode::Space)) move.y += 1;
		if (input.keyDown(avt::KeyCode::LeftShift)) move.y -= 1;
		_cam->processMove(move, dt);

		if (!_cursorVisible) { // free move
			_cam->processOrbit(input.mouseOffset(), dt, true);
		} else if (input.mouseBtnDown(avt::MouseCode::BtnLeft)) { // drag move
			_cam->processOrbit(input.mouseOffset(), dt);
		}

		if (input.keyPressed(avt::KeyCode::Escape)) {
			_cursorVisible = !_cursorVisible;
			if (_cursorVisible) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (input.keyPressed(avt::KeyCode::D0)) {
			_cam->setPosition(avt::Vector3(5.f, 5.f, 5.f));
			_cam->lookAt({});
		}
		if (input.mouseBtnPressed(avt::MouseCode::BtnLeft)) {
		}
	}

	void updateCallback(GLFWwindow* win, float dt) override {
		_time += dt;
		glfwSetWindowTitle(win, std::to_string((int)(1/dt)).c_str());
	}

	void displayCallback(GLFWwindow* win, float dt) override {
		//_renderer.clear();

		_renderer.draw(_scene, _cam.get());
		
		//if (_cursorVisible) avt::StencilPicker::getTargetOnCursor(win);
		//else				avt::StencilPicker::getTargetOnCenter(win);
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		_cam->resize(w, h);
	}

};


int main(int argc, char* argv[]) {
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 0;

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