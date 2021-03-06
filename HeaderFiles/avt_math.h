#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

#include "Matrix.h"
#include "Mat4.h"
#include "Mat3.h"
#include "Mat2.h"

#include "Quaternion.h"

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a > b ? b : a)
#define clamp(v, b1, b2) min(max(v, b1), b2)
#define lerp(a, b, k) ((b - a) * k + a)

namespace avt {

	constexpr float PI = 3.14159265359f;

	inline float toRad(const float deg) {
		return deg * PI / 180;
	}

	inline float toDeg(const float rad) {
		return rad * 180 / PI;
	}

	inline float random() {
		return std::rand() % 10000 / 10000.0f;
	}

	inline float randrange(float lower, float upper) {
		return random() * (upper - lower) + lower;
	}

	inline Vector3 randVector() {
		return { random(), random(), random() };
	}

	inline Vector3 randVector(float lower, float upper) {
		return { randrange(lower, upper), randrange(lower, upper), randrange(lower, upper) };
	}

	// Vectors

	struct Vector2;
	struct Vector3;
	struct Vector4;


	// Matrices

	template<unsigned int N>
	class Matrix;

	class Mat2;
	class Mat3;
	class Mat4;

	// Quaternions
	struct Quaternion;

}