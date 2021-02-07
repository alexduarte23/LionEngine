#include "../HeaderFiles/Vector2.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector4.h"


namespace avt {

	float Vector2::dot(const Vector2& vec) const {
		return x * vec.x + y * vec.y;
	}

	float Vector2::length() const {
		return std::sqrt(x * x + y * y);
	}

	float Vector2::quadrance() const {
		return x * x + y * y;
	}

	Vector2 Vector2::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector2& Vector2::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector2 Vector2::pow(float exp) const {
		return Vector2(std::pow(x, exp), std::pow(y, exp));
	}

	float Vector2::distanceTo(const Vector2& vec) const {
		Vector2 newV(vec.x - x, vec.y - y);
		return newV.length();
	}

	float Vector2::angleTo(const Vector2& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector2 Vector2::operator+() const {
		return Vector2(x, y);
	}

	Vector2 Vector2::operator-() const {
		return Vector2(-x, -y);
	}

	Vector2 Vector2::operator+(const Vector2& vec) const {
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 Vector2::operator+(float num) const {
		return Vector2(x + num, y + num);
	}

	bool Vector2::operator==(const Vector2& vec) const {
		return x == vec.x && y == vec.y;
	}

	bool Vector2::operator!=(const Vector2& vec) const {
		return x != vec.x || y != vec.y;
	}

	float Vector2::operator*(const Vector2& vec) const {
		return dot(vec);
	}

	Vector2 Vector2::operator*(float num) const {
		return Vector2(x * num, y * num);
	}


	Vector2 Vector2::operator-(const Vector2& vec) const {
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 Vector2::operator-(float num) const {
		return Vector2(x - num, y - num);
	}

	Vector2& Vector2::operator+=(const Vector2& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2& Vector2::operator+=(float num) {
		x += num;
		y += num;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2& Vector2::operator-=(float num) {
		x -= num;
		y -= num;
		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& vec) {
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float num) {
		x *= num;
		y *= num;
		return *this;
	}

	Vector2& Vector2::operator/=(float num) {
		x /= num;
		y /= num;
		return *this;
	}

	Vector2 Vector2::operator/(float num) const {
		return Vector2(x / num, y / num);
	}

	Vector2& Vector2::operator=(const Vector2& vec) {
		x = vec.x;
		y = vec.y;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
		os << "(" << vec.x << ", " << vec.y << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector2& vec) {
		is >> vec.x >> vec.y;
		return is;
	}

	Vector3 Vector2::to3D(float z) const {
		return Vector3(x, y, z);
	}

	Vector4 Vector2::to4D(float z, float w) const {
		return Vector4(x, y, z, w);
	}

}