#include <iostream>
#include <vector>

#include "../HeaderFiles/Engine.h"



class MyApp : public avt::App {
private:
	avt::Shader _shader, _shaderFire, _shaderHUD, _shaderBG;
	avt::Renderer _renderer;
	avt::UniformBuffer _ub;
	avt::Scene _scene, _HUD;

	avt::Manager<avt::Mesh> _meshes;
	avt::Manager<avt::Camera> _cams;

	bool _cursorVisible = false;

	avt::ParticleEmitter* _fireEmitter = nullptr;
	avt::Background* _background = nullptr;
	avt::SceneNode* _crosshair = nullptr;
	std::string _activeCam = "per";

	double _time = 0;

	void createScene() {
		avt::StencilPicker::enable();

		// MESHES
		auto cubeM = _meshes.add("cube", new avt::Mesh("./Resources/Objects/cube_vtn_flat.obj"));
		cubeM->setup();

		auto colorCubeM = _meshes.add("colorCube", new avt::Mesh("./Resources/Objects/colourscube.obj"));
		colorCubeM->setup();

		// SCENE
		_scene.setShader(&_shader);
		
		// SCENE - BACKGROUND
		_background = new avt::Background({.5f, .5f, .5f});
		_scene.addNode(_background);
		_background->setShader(&_shaderBG);

		// SCENE - OPAQUE
		auto cube = _scene.createNode(colorCubeM);

		// SCENE - TRANSPARENT
		_fireEmitter = new avt::FireEmitter();
		avt::StencilPicker::addTarget(_fireEmitter, "fire");
		_fireEmitter->setShader(&_shaderFire);
		_fireEmitter->scale({ .9f, .9f, .9f });
		_fireEmitter->translate({ 5.f, 0, 0 });
		_scene.addNode(_fireEmitter); // scene deletes nodes when destroyed

		// HUD
		_HUD.setShader(&_shaderHUD);

		_crosshair = _HUD.addNode(new avt::HUDElement("Resources/textures/crosshair161.png"));
		_crosshair->scale({ .5f, .5f, .5f });
	}

		

	void processKeyInput(GLFWwindow* win, double dt) {
		avt::Vector3 move;
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) move.z += 1;
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) move.z -= 1;
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) move.x -= 1;
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) move.x += 1;
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) move.y += 1;
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) move.y -= 1;

		_cams.get("per")->processMove(move, (float)dt);
	}

	void processMouseMovement(GLFWwindow* win, const avt::Vector2& lastCursor, const avt::Vector2& newCursor, double  dt) {
		auto offset = newCursor - lastCursor;
		if (!_cursorVisible) { // free move
			_cams.get("per")->processOrbit(offset, (float)dt, true);

		} else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { // drag move
			_cams.get("per")->processOrbit(offset, (float)dt);

		}

	}

	void createShaders() {
		_ub.create(2 * (long long)avt::LayoutElement::getTypeSize(avt::ShaderDataType::MAT4), 0); // change
		_ub.unbind();

		// create regular mesh shader
		avt::ShaderParams params;
		params.setVertexShader("./Resources/shaders/basic-vs.glsl")
			.setFragmentShader("./Resources/shaders/basic-fs.glsl")
			.addInputs({ "position", "texCoord", "normal", "color" })
			.addUniform("ModelMatrix")
			.addUniformBlock("CameraMatrices", 0);
		_shader.create(params);

		// create fire shader
		params.clear()
			.setVertexShader("./Resources/shaders/particles-vs.glsl")
			.setFragmentShader("./Resources/shaders/fire-fs.glsl")
			.addInputs({ "in_vertex", "in_texCoord", "in_pos", "in_color", "in_size", "in_rot" })
			.addUniform("ModelMatrix")
			.addTextures({ "in_texture", "in_dissolveMap" })
			.addUniformBlock("SharedMatrices", 0);
		_shaderFire.create(params);

		// create hud shader
		params.clear()
			.setVertexShader("./Resources/HUDshaders/hud-vs.glsl")
			.setFragmentShader("./Resources/HUDshaders/hud-fs.glsl")
			.addInputs({ "inPosition", "inTexcoord", "inColor" })
			.addUniform("ModelMatrix")
			.addTexture("inTexture", 0)
			.addUniformBlock("CameraMatrices", 0);
		_shaderHUD.create(params);

		// create background shader
		params.clear()
			.setVertexShader("./Resources/bgShaders/bg-vs.glsl")
			.setFragmentShader("./Resources/bgShaders/bg-fs.glsl")
			.addInputs({ "inPosition", "inColor" });
		_shaderBG.create(params);
	}


	void createCams(GLFWwindow* win) {
		int winx, winy;
		glfwGetWindowSize(win, &winx, &winy);

		float aspect = winx / (float)winy;

		auto camP = new avt::PerspectiveCamera(60.f, aspect, 0.1f, 200.0f, avt::Vector3(0, 5.f, 10.f));
		auto camHUD = new avt::OrthographicCamera(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, 0.1f, 100.0f, avt::Vector3(0, 0, 10.f));
		camP->setMoveSpeed(8.f);

		_cams.add("per", camP);
		_cams.add("HUD", camHUD);


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

		_fireEmitter->update(dt);

		if (avt::StencilPicker::getLastPick().first == nullptr && avt::StencilPicker::getLastPick().second == "") {
			_crosshair->setScale({ .5f, .5f, .5f });
		} else {
			_crosshair->setScale({ .8f, .8f, .8f });
		}
	}

	void displayCallback(GLFWwindow* win, double dt) override {
		_renderer.clear();

		_scene.draw(_ub, _cams.get(_activeCam));

		if (_cursorVisible) avt::StencilPicker::getTargetOnCursor(win);
		else				avt::StencilPicker::getTargetOnCenter(win);

		_HUD.draw(_ub, _cams.get("HUD"));
	}

	void windowResizeCallback(GLFWwindow* win, int w, int h) override {
		glViewport(0, 0, w, h);
		_cams.get("per")->resize(w, h);
		_cams.get("HUD")->resize(w, h);
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
			if (_cams.size() == 3) {
				_cams.get("per")->setPosition(avt::Vector3(5.f, 5.f, 5.f));
				_cams.get("per")->lookAt({});
			}
			break;
		}
	}

	void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			auto target = avt::StencilPicker::getLastPick();
			if (target.second == "fire") {
				_fireEmitter->toggle();
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