#pragma once

#include <iostream>


namespace avt {

	struct Vector3;
	struct Vector4;
	class Mat4;

	struct Quaternion {
	private:
		static constexpr float THRESHOLD = (float)1.0e-5;

	public:
		float t, x, y, z;

		Quaternion(float t=0, float x=0, float y=0, float z=0) 
			: t(t), x(x), y(y), z(z) {}

		Quaternion(Vector3 axis, float rad);

		~Quaternion() {}


		Vector3 getAxis() const;

		float getAngle() const;

		void clean();

		float quadrance() const;

		float length() const;

		void normalize();

		Quaternion normalized() const;

		Quaternion conj() const;

		Quaternion inv() const;

		Quaternion operator+(const Quaternion& q) const;

		Quaternion& operator+=(const Quaternion& q);

		Quaternion operator-(const Quaternion& q) const;

		Quaternion& operator-=(const Quaternion& q);

		Quaternion operator*(const float s) const;

		Quaternion& operator*=(const float s);

		Quaternion operator/(const float s) const;

		Quaternion& operator/=(const float s);

		Quaternion operator*(const Quaternion& q) const;

		Quaternion& operator*=(const Quaternion& q);

		Mat4 toMat() const;

		Quaternion lerp(const Quaternion& qEnd, float k) const;

		Quaternion slerp(const Quaternion& qEnd, float k) const;

		bool operator==(const Quaternion& q1) const;

		bool operator!=(const Quaternion& q1) const;

		friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
			os << "(" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
			return os;
		}

		friend Quaternion operator*(const float s, Quaternion& q) {
			return q * s;
		}

		friend Quaternion operator+(const float s, Quaternion& q) {
			return q + s;
		}
	};

}