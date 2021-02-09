#pragma once

#include <iostream>
#include <math.h>


namespace avt {

	struct Vector2;
	struct Vector4;
	struct Quaternion;

	struct Vector3 {
		float x, y, z;

		Vector3(float x = 0, float y = 0, float z = 0)
			: x(x), y(y), z(z) {}

		Vector3(const Vector3& vec)
			: x(vec.x), y(vec.y), z(vec.z) {}

		Vector3(const Vector2& vec, float z = 0);

		~Vector3() {}

		Vector3& operator=(const Vector3& vec);

		Vector2 to2D() const;

		Vector4 to4D(float w = 1.f) const;

		float dot(const Vector3& vec) const;

		Vector3 cross(const Vector3& vec) const;

		float length() const;

		// length squared
		float quadrance() const;

		Vector3 normalized() const;

		Vector3& normalize();

		Vector3 pow(float exp) const;

		float distanceTo(const Vector3& vec) const;

		float angleTo(const Vector3& vec) const;

		Vector3 rotateOnAxis(const Vector3& axis, float rad) const;
		
		Vector3 rotateOnQuat(const Quaternion& q) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);

		friend std::istream& operator>>(std::istream& is, Vector3& vec);

		Vector3 operator+(const Vector3& vec) const;

		Vector3 operator+(float num) const;

		Vector3 operator-(const Vector3& vec) const;

		Vector3 operator-(float num) const;

		Vector3 operator+() const;

		Vector3 operator-() const;

		bool operator==(const Vector3& vec) const;

		bool operator!=(const Vector3& vec) const;

		float operator*(const Vector3& vec) const;

		Vector3 operator*(float num) const;

		Vector3& operator+=(const Vector3& vec);

		Vector3& operator+=(float num);

		Vector3& operator-=(const Vector3& vec);

		Vector3& operator-=(float num);

		Vector3& operator*=(const Vector3& vec);

		Vector3& operator*=(float num);

		Vector3& operator/=(float num);

		Vector3 operator/(float num) const;

		friend Vector3 operator*(float num, const Vector3& vec) {
			return vec * num;
		}

		friend Vector3 operator+(float num, const Vector3& vec) {
			return vec + num;
		}

		static float quadProd(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);
	};

}