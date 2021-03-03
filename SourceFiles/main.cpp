#include <iostream>
#include <vector>
#include <memory>


#include "../HeaderFiles/Engine.h"


class MyApp : public avt::App {
private:
	std::shared_ptr<avt::Material> _mtl, _mtl2;
	avt::Renderer _renderer;
	std::shared_ptr<avt::UniformBuffer> _ub;
	avt::Scene _scene;
	std::unique_ptr<avt::Camera> _cam;

	static constexpr int FRAME_N = 30;
	float _frames[FRAME_N] = { 0 };

	void createScene() {
		//avt::StencilPicker::enable();

		// cube_vtn_flat
		auto mesh = std::make_shared<avt::Mesh>("./Resources/Objects/colourscube.obj");
		auto mesh2 = std::make_shared<avt::Mesh>("./Resources/Objects/cube_vtn_flat.obj");
		std::shared_ptr<avt::RenderMesh> rend[4];
		rend[0] = std::make_shared<avt::RenderMesh>(mesh, _mtl);
		rend[1] = std::make_shared<avt::RenderMesh>(mesh, _mtl2);
		rend[2] = std::make_shared<avt::RenderMesh>(mesh2, _mtl);
		rend[3] = std::make_shared<avt::RenderMesh>(mesh2, _mtl2);

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				auto node = _scene.createNode(rend[j%4]);
				node->translate({ -i*2.5f, 0, -j*2.5f });
			}
		}
	}


	void createShaders() {
		// create regular mesh shader
		avt::ShaderParams params;
		params.setVertexShader("./Resources/shaders/basic-vs.glsl")
			.setFragmentShader("./Resources/shaders/basic-fs.glsl")
			.useModelMatrix("ModelMatrix")
			.addUniformBlock("CameraMatrices", 0);
		auto shader = std::make_shared<avt::Shader>(params);
		
		params.setVertexShader("./Resources/shaders/test-vs.glsl");
		auto shaderT = std::make_shared<avt::Shader>(params);

		auto& layout = shader->getInputLayout();
		std::cout << "GOOD: " << layout.wellFormed() << std::endl;
		for (auto& attr : layout.getAttrs())
			std::cout << "Name: " << attr.name << " Loaction: " << attr.location << " Count: " << attr.length << std::endl;

		_mtl = std::make_shared<avt::Material>(shader);
		_mtl2 = std::make_shared<avt::Material>(shader);
	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);
		float aspect = winx / (float)winy;

		_cam.reset(new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(5.f, 5.f, 5.f)));
		_cam->lookAt({});
		_cam->setMoveSpeed(20.f);
		_cam->setOrbitSpeed(.002f);

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
		avt::Input::setCursorMode(avt::CursorMode::Captured);
	}

	void processInput(float dt) {
		avt::Vector3 move;
		if (avt::Input::keyDown(avt::KeyCode::W)) move.z += 1;
		if (avt::Input::keyDown(avt::KeyCode::S)) move.z -= 1;
		if (avt::Input::keyDown(avt::KeyCode::A)) move.x -= 1;
		if (avt::Input::keyDown(avt::KeyCode::D)) move.x += 1;
		if (avt::Input::keyDown(avt::KeyCode::Space)) move.y += 1;
		if (avt::Input::keyDown(avt::KeyCode::LeftShift)) move.y -= 1;
		_cam->processMove(move, dt);

		if (avt::Input::cursorMode() == avt::CursorMode::Captured) { // free move
			_cam->processOrbit(avt::Input::mouseOffset(), dt, true);
		} else if (avt::Input::mouseBtnDown(avt::MouseCode::BtnLeft)) { // drag move
			_cam->processOrbit(avt::Input::mouseOffset(), dt);
		}

		if (avt::Input::keyPressed(avt::KeyCode::Escape)) {
			if (avt::Input::cursorMode() == avt::CursorMode::Captured)
				avt::Input::setCursorMode(avt::CursorMode::Normal);
			else
				avt::Input::setCursorMode(avt::CursorMode::Captured);
		}
		if (avt::Input::keyPressed(avt::KeyCode::D0)) {
			_cam->setPosition(avt::Vector3(5.f, 5.f, 5.f));
			_cam->lookAt({});
		}
		if (avt::Input::mouseBtnPressed(avt::MouseCode::BtnLeft)) {
		}
	}

	void displayFPS(GLFWwindow* win, float dt) {
		float avg = 0;
		for (int i = 0; i < FRAME_N - 1; i++) {
			_frames[i] = _frames[i + 1];
			avg += _frames[i];
		}
		_frames[FRAME_N - 1] = dt;
		avg = (avg + dt) / (float)FRAME_N;

		glfwSetWindowTitle(win, std::to_string((int)(1.f/avg)).c_str());
	}

	void onUpdate(GLFWwindow* win, float dt) override {
		processInput(dt);
		displayFPS(win, dt);
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