#pragma once

#include <GL/glew.h>
#include <memory>

#include "avt_math.h"
#include "UniformBuffer.h"


namespace avt {

	class Camera {
	protected:

		static constexpr float MOVE_SPEED = 3.f;
		static constexpr float ORBIT_SPEED = .05f;
		static constexpr float MARGIN_ANGLE = .15f;

		Vector3 _pos;
		Vector3 _worldUp;
		Vector3 _front, _up, _right;

		float _moveSpeed, _orbitSpeed, _margin;

		Mat4 _viewM, _projM;

		std::shared_ptr<UniformBuffer> _cameraUBO;

		void updateView();

	protected:
		Camera(Vector3 pos = Vector3{}, Vector3 up = Vector3(0,1.0f,0))
			: _pos(pos), _worldUp(up), _front(0,0,-1.f), _moveSpeed(MOVE_SPEED), _orbitSpeed(ORBIT_SPEED), _margin(MARGIN_ANGLE) {

			updateView();
		}

	public:
		virtual ~Camera() {}

		const Mat4& viewMatrix() const;

		const Mat4& projMatrix() const;

		virtual void resize(int w, int h) {}

		void setOrbitSpeed(float speed) {
			_orbitSpeed = speed;
		}

		void setMoveSpeed(float speed) {
			_moveSpeed = speed;
		}

		void setVerticalMargin(float margin) {
			_margin = margin;
		}

		void setPosition(const Vector3& pos) {
			_pos = pos;
			updateView();
		}

		const Vector3& position() {
			return _pos;
		}

		void setUp(const Vector3& up) {
			_worldUp = up;
			updateView();
		}

		const Vector3& up() {
			return _worldUp;
		}

		void lookAt(const Vector3& target) {
			_front = (target - _pos).normalized();
			updateView();
		}

		void processMove(const Vector3& movement, float dt);

		void processOrbit(const Vector2& offset, float dt, bool invert=false);

		void setUBO(const std::shared_ptr<UniformBuffer>& ubo) {
			_cameraUBO = ubo;
		}

		const std::shared_ptr<UniformBuffer>& getUBO() const {
			return _cameraUBO;
		}

	};

}