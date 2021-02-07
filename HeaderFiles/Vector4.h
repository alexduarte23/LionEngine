#pragma once

#include <iostream>
#include <math.h>
#include <GL/glew.h>


namespace avt {

	struct Vector2;
	struct Vector3;

	struct Vector4 {
	public:
		float x, y, z, w;

		Vector4(float x = 0, float y = 0, float z = 0, float w = 0)
			: x(x), y(y), z(z), w(w) {}

		Vector4(const Vector4& vec)
			: x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

		Vector4(const Vector2& vec, float z = 0, float w = 0);

		Vector4(const Vector3& vec, float w = 0);

		~Vector4() {}

		Vector4& operator=(const Vector4& vec);

		Vector2 to2D() const;

		Vector3 to3D() const;

		float dot(const Vector4& vec) const;

		float length() const;

		// length squared
		float quadrance() const;

		Vector4 normalized() const;

		Vector4& normalize();

		Vector4 pow(float exp) const;

		float distanceTo(const Vector4& vec) const;

		float angleTo(const Vector4& vec) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);

		friend std::istream& operator>>(std::istream& is, Vector4& vec);

		Vector4 operator+(const Vector4& vec) const;

		Vector4 operator+(float num) const;

		Vector4 operator-(const Vector4& vec) const;

		Vector4 operator-(float num) const;

		Vector4 operator+() const;

		Vector4 operator-() const;

		bool operator==(const Vector4& vec) const;

		bool operator!=(const Vector4& vec) const;

		float operator*(const Vector4& vec) const;

		Vector4 operator*(float num) const;

		Vector4& operator+=(const Vector4& vec);

		Vector4& operator+=(float num);

		Vector4& operator-=(const Vector4& vec);

		Vector4& operator-=(float num);

		Vector4& operator*=(const Vector4& vec);

		Vector4& operator*=(float num);

		Vector4& operator/=(float num);

		Vector4 operator/(float num) const;

		friend Vector4 operator*(float num, const Vector4& vec) {
			return vec * num;
		}

		friend Vector4 operator+(float num, const Vector4& vec) {
			return vec + num;
		}
	};

}


