#include "../HeaderFiles/Vector3.h"

#include "../HeaderFiles/Vector2.h"
#include "../HeaderFiles/Vector4.h"
#include "../HeaderFiles/Mat3.h"
#include "../HeaderFiles/Mat4.h"
#include "../HeaderFiles/Quaternion.h"


namespace avt {

	Vector3::Vector3(const Vector2& vec, float z)
		: x(vec.x), y(vec.y), z(z) {}

	float Vector3::dot(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	Vector3 Vector3::cross(const Vector3& vec) const {
		float newX = y * vec.z - z * vec.y;
		float newY = z * vec.x - x * vec.z;
		float newZ = x * vec.y - y * vec.x;

		return Vector3(newX, newY, newZ);
	}

	float Vector3::length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	float Vector3::quadrance() const {
		return x * x + y * y + z * z;
	}

	Vector3 Vector3::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector3& Vector3::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector3 Vector3::pow(float exp) const {
		return Vector3(std::pow(x, exp), std::pow(y, exp), std::pow(z, exp));
	}

	float Vector3::distanceTo(const Vector3& vec) const {
		Vector3 newV(vec.x - x, vec.y - y, vec.z - z);
		return newV.length();
	}

	float Vector3::angleTo(const Vector3& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector3 Vector3::rotateOnAxis(const Vector3& axis, float rad) const {
		Mat3 K = Mat3::dual(axis);
		Mat3 R = Mat3::identity() + sin(rad) * K + (1 - cos(rad)) * (K * K);
		return R * *this;
	}

	Vector3 Vector3::rotateOnQuat(const Quaternion& q) const {
		return (q.toMat() * (*this).to4D()).to3D();
	}

	Vector3 Vector3::operator+() const {
		return Vector3(x, y, z);
	}

	Vector3 Vector3::operator-() const {
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator+(const Vector3& vec) const {
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 Vector3::operator+(float num) const {
		return Vector3(x + num, y + num, z + num);
	}

	bool Vector3::operator==(const Vector3& vec) const {
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool Vector3::operator!=(const Vector3& vec) const {
		return x != vec.x || y != vec.y || z != vec.z;
	}

	float Vector3::operator*(const Vector3& vec) const {
		return dot(vec);
	}

	Vector3 Vector3::operator*(float num) const {
		return Vector3(x * num, y * num, z * num);
	}


	Vector3 Vector3::operator-(const Vector3& vec) const {
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 Vector3::operator-(float num) const {
		return Vector3(x - num, y - num, z - num);
	}

	Vector3& Vector3::operator+=(const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& Vector3::operator+=(float num) {
		x += num;
		y += num;
		z += num;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& Vector3::operator-=(float num) {
		x -= num;
		y -= num;
		z -= num;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	Vector3& Vector3::operator/=(float num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}

	Vector3 Vector3::operator/(float num) const {
		return Vector3(x / num, y / num, z / num);
	}

	Vector3& Vector3::operator=(const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector3& vec) {
		is >> vec.x >> vec.y >> vec.z;
		return is;
	}

	Vector2 Vector3::to2D() const {
		return Vector2(x, y);
	}

	Vector4 Vector3::to4D(float w) const {
		return Vector4(x, y, z, w);
	}


	float Vector3::quadProd(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d) {
		return (a * c) * (b * d) - (a * d) * (b * c);
	}

}