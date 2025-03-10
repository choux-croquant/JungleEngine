#pragma once
#include <cmath>
#include "Vector.h"

class Quaternion {
public:
	float W;
	float X;
	float Y;
	float Z;

public:
	Quaternion(float w=1, float x=0, float y=0, float z=0) :W(w), X(x), Y(y), Z(z) {}

	Quaternion operator*(const Quaternion& rhs)const {
		return Quaternion(
			W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z,
			W * rhs.X + X * rhs.W + Y * rhs.Z - Z * rhs.Y,
			W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X,
			W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W
		);
	}

	Quaternion inverse() const {
		return Quaternion(W, -X, -Y, -Z);
	}

	FVector RotateVector(FVector pos) {
		Quaternion vector(0, pos.X, pos.Y, pos.Z);
		Quaternion roatatedVector = (*this) * vector * this->inverse();

		pos.X = roatatedVector.X;
		pos.Y = roatatedVector.Y;
		pos.Z = roatatedVector.Z;

		return pos;
	}

	Quaternion Normalize() {
		float mag = sqrtf(W * W + X * X + Y * Y + Z * Z);
		if (mag > 0) {
			W /= mag;
			X /= mag;
			Y /= mag;
			Z /= mag;
		}
	}

	static Quaternion Rotate(float angle, float x, float y, float z) {
		float halfAngle = angle / 2.0f;
		float sinHalfAngle = sin(halfAngle);

		return Quaternion(
			cos(halfAngle),
			sinHalfAngle * x,
			sinHalfAngle * y,
			sinHalfAngle * z
		);
	}

	static Quaternion FromEulerAngles(FVector eulerAngles) {
		float cx = cos(eulerAngles.X / 2);
		float sx = sin(eulerAngles.X / 2);
		float cy = cos(eulerAngles.Y / 2);
		float sy = sin(eulerAngles.Y / 2);
		float cz = cos(eulerAngles.Z / 2);
		float sz = sin(eulerAngles.Z / 2);

		return Quaternion(
			cx * cy * cz + sx * sy * sz,
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz + sx * cy * sz,
			cx * cy * sz - sx * sy * cz
		);
	}

	static Quaternion RotateX(float angle) {
		return Rotate(angle, 1.0f, 0.0f, 0.0f);
	}

	static Quaternion RotateY(float angle) {
		return Rotate(angle, 0.0f, 1.0f, 0.0f);
	}

	static Quaternion RotateZ(float angle) {
		return Rotate(-angle, 0.0f, 0.0f, 1.0f);
	}

	static Quaternion Rotate(FVector axis, float angle) {
		return Rotate(angle, axis.X, axis.Y, axis.Z);
	}
};