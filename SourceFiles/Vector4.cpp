#include "../HeaderFiles/Vector4.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector2.h"


namespace avt {

	Vector4::Vector4(const Vector2& vec, float z, float w)
		: x(vec.x), y(vec.y), z(z), w(w) {}

	Vector4::Vector4(const Vector3& vec, float w)
		: x(vec.x), y(vec.y), z(vec.z), w(w) {}

	float Vector4::dot(const Vector4& vec) const {
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	float Vector4::length() const {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	float Vector4::quadrance() const {
		return x * x + y * y + z * z + w * w;
	}

	Vector4 Vector4::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector4& Vector4::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector4 Vector4::pow(float exp) const {
		return Vector4(std::pow(x, exp), std::pow(y, exp), std::pow(z, exp), std::pow(w, exp));
	}

	float Vector4::distanceTo(const Vector4& vec) const {
		Vector4 newV(vec.x - x, vec.y - y, vec.z - z, vec.w - w);
		return newV.length();
	}

	float Vector4::angleTo(const Vector4& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector4 Vector4::operator+() const {
		return Vector4(x, y, z, w);
	}

	Vector4 Vector4::operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	Vector4 Vector4::operator+(const Vector4& vec) const {
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	Vector4 Vector4::operator+(float num) const {
		return Vector4(x + num, y + num, z + num);
	}

	bool Vector4::operator==(const Vector4& vec) const {
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	bool Vector4::operator!=(const Vector4& vec) const {
		return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
	}

	float Vector4::operator*(const Vector4& vec) const {
		return dot(vec);
	}

	Vector4 Vector4::operator*(float num) const {
		return Vector4(x * num, y * num, z * num, w * num);
	}


	Vector4 Vector4::operator-(const Vector4& vec) const {
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	Vector4 Vector4::operator-(float num) const {
		return Vector4(x - num, y - num, z - num, w - num);
	}

	Vector4& Vector4::operator+=(const Vector4& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}

	Vector4& Vector4::operator+=(float num) {
		x += num;
		y += num;
		z += num;
		w += num;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	Vector4& Vector4::operator-=(float num) {
		x -= num;
		y -= num;
		z -= num;
		w -= num;
		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
		return *this;
	}

	Vector4& Vector4::operator*=(float num) {
		x *= num;
		y *= num;
		z *= num;
		w *= num;
		return *this;
	}

	Vector4& Vector4::operator/=(float num) {
		x /= num;
		y /= num;
		z /= num;
		w /= num;
		return *this;
	}

	Vector4 Vector4::operator/(float num) const {
		return Vector4(x / num, y / num, z / num, w / num);
	}

	Vector4& Vector4::operator=(const Vector4& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector4& vec) {
		is >> vec.x >> vec.y >> vec.z >> vec.w;
		return is;
	}

	Vector2 Vector4::to2D() const {
		return Vector2(x, y);
	}

	Vector3 Vector4::to3D() const {
		return Vector3(x, y, z);
	}

}