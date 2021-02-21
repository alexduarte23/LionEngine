#include <iostream>
#include <vector>
#include <memory>


#include "../HeaderFiles/Engine.h"



class MyApp : public avt::App {
private:
	std::shared_ptr<avt::Shader> _shader;
	avt::Renderer _renderer;
	std::shared_ptr<avt::UniformBuffer> _ub;
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
		cubeM->setShader(_shader);
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
		_shader = std::make_shared<avt::Shader>(params);

	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		float aspect = winx / (float)winy;

		_cam.reset(new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(5.f, 5.f, 5.f)));
		_cam->lookAt({});
		_cam->setMoveSpeed(12.f);
		_cam->setOrbitSpeed(.45f);

		_ub = std::make_shared<avt::UniformBuffer>(2 * (long long)avt::LayoutElement::getTypeSize(avt::ShaderDataType::MAT4), 0);
		_ub->unbind();
		_cam->setUBO(_ub);
	}

public:

	MyApp() : avt::App() {}

	~MyApp() {}

	void onInit(GLFWwindow* win) override {
		createCams(win);
		createShaders();
		createScene();
	}

	void processInput(GLFWwindow* win, float dt) {
		avt::Vector3 move;
		if (avt::Input::keyDown(avt::KeyCode::W)) move.z += 1;
		if (avt::Input::keyDown(avt::KeyCode::S)) move.z -= 1;
		if (avt::Input::keyDown(avt::KeyCode::A)) move.x -= 1;
		if (avt::Input::keyDown(avt::KeyCode::D)) move.x += 1;
		if (avt::Input::keyDown(avt::KeyCode::Space)) move.y += 1;
		if (avt::Input::keyDown(avt::KeyCode::LeftShift)) move.y -= 1;
		_cam->processMove(move, dt);

		if (!_cursorVisible) { // free move
			_cam->processOrbit(avt::Input::mouseOffset(), dt, true);
		} else if (avt::Input::mouseBtnDown(avt::MouseCode::BtnLeft)) { // drag move
			_cam->processOrbit(avt::Input::mouseOffset(), dt);
		}

		if (avt::Input::keyPressed(avt::KeyCode::Escape)) {
			_cursorVisible = !_cursorVisible;
			if (_cursorVisible) glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else				glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (avt::Input::keyPressed(avt::KeyCode::D0)) {
			_cam->setPosition(avt::Vector3(5.f, 5.f, 5.f));
			_cam->lookAt({});
		}
		if (avt::Input::mouseBtnPressed(avt::MouseCode::BtnLeft)) {
		}
	}

	void onUpdate(GLFWwindow* win, float dt) override {
		processInput(win, dt);

		_time += dt;
		glfwSetWindowTitle(win, std::to_string((int)(1/dt)).c_str());
	}

	void onDisplay(GLFWwindow* win, float dt) override {
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