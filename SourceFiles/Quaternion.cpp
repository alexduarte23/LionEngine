#include "../HeaderFiles/Quaternion.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector4.h"
#include "../HeaderFiles/Mat4.h"

namespace avt {

	Quaternion::Quaternion(Vector3 axis, float rad) {
		Vector3 axisn = axis.normalized();
		Vector3 v = sin(rad / 2.0f) * axisn;

		t = cos(rad / 2.0f);
		x = v.x;
		y = v.y;
		z = v.z;

		clean();
		normalize();
	}

	Vector3 Quaternion::getAxis() const {
		Quaternion qn = normalized();
		float s = sqrt(1.0f - qn.t * qn.t);
		if (s < THRESHOLD) {
			return Vector3(1.0f, 0, 0);
		} else {
			return Vector3(qn.x, qn.y, qn.z) / s;
		}
	}

	float Quaternion::getAngle() const {
		return 2.0f * acos(t);
	}


	void Quaternion::clean() {
		if (fabs(t) < THRESHOLD) t = 0.0f;
		if (fabs(x) < THRESHOLD) x = 0.0f;
		if (fabs(y) < THRESHOLD) y = 0.0f;
		if (fabs(z) < THRESHOLD) z = 0.0f;
	}

	float Quaternion::quadrance() const	{
		return t * t + x * x + y * y + z * z;
	}

	float Quaternion::length() const {
		return sqrt(quadrance());
	}

	void Quaternion::normalize() {
		*this /= length();
	}

	Quaternion Quaternion::normalized() const {
		return *this / length();
	}

	Quaternion Quaternion::conj() const {
		return Quaternion(t, -x, -y, -z);
	}

	Quaternion Quaternion::inv() const {
		return conj() / quadrance();
	}

	Quaternion Quaternion::operator+(const Quaternion& q) const {
		return Quaternion(t + q.t, x + q.x, y + q.y, z + q.z);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q) {
		t += q.t;
		x += q.x;
		y += q.y;
		z += q.z;

		return *this;
	}

	Quaternion Quaternion::operator-(const Quaternion& q) const {
		return Quaternion(t - q.t, x - q.x, y - q.y, z - q.z);
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q) {
		t -= q.t;
		x -= q.x;
		y -= q.y;
		z -= q.z;

		return *this;
	}

	Quaternion Quaternion::operator*(const float s) const {
		return Quaternion(t * s, x * s, y * s, z * s);
	}

	Quaternion& Quaternion::operator*=(const float s) {
		t *= s;
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	Quaternion Quaternion::operator/(const float s) const {
		return Quaternion(t / s, x / s, y / s, z / s);
	}

	Quaternion& Quaternion::operator/=(const float s) {
		t /= s;
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}

	Quaternion Quaternion::operator*(const Quaternion& q) const {
		Quaternion res;
		res.t = t * q.t - x * q.x - y * q.y - z * q.z;
		res.x = t * q.x + x * q.t + y * q.z - z * q.y;
		res.y = t * q.y + y * q.t + z * q.x - x * q.z;
		res.z = t * q.z + z * q.t + x * q.y - y * q.x;
		return res;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q) {
		Quaternion res;
		res.t = t * q.t - x * q.x - y * q.y - z * q.z;
		res.x = t * q.x + x * q.t + y * q.z - z * q.y;
		res.y = t * q.y + y * q.t + z * q.x - x * q.z;
		res.z = t * q.z + z * q.t + x * q.y - y * q.x;
		
		t = res.t;
		x = res.x;
		y = res.y;
		z = res.z;

		return *this;
	}

	Mat4 Quaternion::toMat() const {
		Quaternion qn = normalized();
		Mat4 mat;

		float xx = qn.x * qn.x;
		float xy = qn.x * qn.y;
		float xz = qn.x * qn.z;
		float xt = qn.x * qn.t;
		float yy = qn.y * qn.y;
		float yz = qn.y * qn.z;
		float yt = qn.y * qn.t;
		float zz = qn.z * qn.z;
		float zt = qn.z * qn.t;

		mat[0] = 1.0f - 2.0f * (yy + zz);
		mat[1] = 2.0f * (xy + zt);
		mat[2] = 2.0f * (xz - yt);
		mat[3] = 0.0f;

		mat[4] = 2.0f * (xy - zt);
		mat[5] = 1.0f - 2.0f * (xx + zz);
		mat[6] = 2.0f * (yz + xt);
		mat[7] = 0.0f;

		mat[8] = 2.0f * (xz + yt);
		mat[9] = 2.0f * (yz - xt);
		mat[10] = 1.0f - 2.0f * (xx + yy);
		mat[11] = 0.0f;

		mat[12] = 0.0f;
		mat[13] = 0.0f;
		mat[14] = 0.0f;
		mat[15] = 1.0f;

		mat.clean();
		return mat;
	}

	Quaternion Quaternion::lerp(const Quaternion& qEnd, float k) const {
		float cosangle = x * qEnd.x + y * qEnd.y + z * qEnd.z + t * qEnd.t;
		float k0 = 1.0f - k;
		float k1 = (cosangle > 0) ? k : -k;
		
		return ((*this * k0) + (qEnd * k1)).normalized();
	}

	Quaternion Quaternion::slerp(const Quaternion& qEnd, float k) const {
		float angle = acos(x * qEnd.x + y * qEnd.y + z * qEnd.z + t * qEnd.t);
		if (angle == 0) return *this;

		float k0 = sin((1 - k) * angle) / sin(angle);
		float k1 = sin(k * angle) / sin(angle);
		
		return ((*this * k0) + (qEnd * k1)).normalized();
	}

	bool Quaternion::operator==(const Quaternion& q) const {
		return (fabs(t - q.t) < THRESHOLD && fabs(x - q.x) < THRESHOLD &&
			fabs(y - q.y) < THRESHOLD && fabs(z - q.z) < THRESHOLD);
	}

	bool Quaternion::operator!=(const Quaternion& q) const {
		return (fabs(t - q.t) >= THRESHOLD || fabs(x - q.x) >= THRESHOLD ||
			fabs(y - q.y) >= THRESHOLD || fabs(z - q.z) >= THRESHOLD);
	}

}