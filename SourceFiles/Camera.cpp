#include "../HeaderFiles/Camera.h"

#include "../HeaderFiles/UniformBuffer.h"

namespace avt {

	void Camera::updateView() {
		_right = (_front.cross(_worldUp)).normalized();
		_up = _right.cross(_front);

		_viewM = Mat4{
			_right.x, _up.x,-_front.x, 0.0f,
			_right.y, _up.y,-_front.y, 0.0f,
			_right.z, _up.z,-_front.z, 0.0f,
			-_pos.dot(_right), -_pos.dot(_up), _pos.dot(_front), 1.0f }; //column major
	}

	const Mat4& Camera::viewMatrix() const {
		return _viewM;
	}

	const Mat4& Camera::projMatrix() const {
		return _projM;
	}

	void Camera::processMove(const Vector3& movement, float dt) {
		auto offsets = movement.normalized() * _moveSpeed * dt;

		_pos += _right * offsets.x + _up * offsets.y + _front * offsets.z;

		updateView();
	}

	void Camera::processOrbit(const Vector2& offset , float dt, bool invert) {
		int direction = invert ? -1 : 1;
		float yaw = direction * offset.x * _orbitSpeed * dt;
		float pitch = direction * offset.y * _orbitSpeed * dt;

		float diff = _front.angleTo(_worldUp);
		if (diff - pitch < _margin)
			pitch = diff - _margin;
		else if (PI - diff + pitch < _margin)
			pitch = - (PI - diff - _margin);

		auto q = Quaternion(_right, pitch) * Quaternion(_up, yaw);
		_front = _front.rotateOnQuat(q);

		updateView();
	}

}