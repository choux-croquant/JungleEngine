#pragma once
#include <cmath>

struct FVector
{
	float X;
	float Y;
	float Z;

	FVector() : X(0), Y(0), Z(0) {}
	FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

	float Dot(const FVector& rhs) {
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
	};
	FVector Cross(const FVector& rhs) const {
		return FVector(Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X);
	};
	float Length() const {
		return sqrt(X * X + Y * Y + Z * Z);
	};
	FVector Normalize() {
		float len = Length();

		if (len > 0) {
			X /= len;
			Y /= len;
			Z /= len;
		}

        return FVector(X,Y,Z);
	};

    // Operator overloading
    FVector operator+(const FVector& rhs) const {
        return FVector(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
    }
    FVector operator-(const FVector& rhs) const {
        return FVector(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
    }
    FVector operator*(float scalar) const {
        return FVector(X * scalar, Y * scalar, Z * scalar);
    }
    FVector operator/(float scalar) const {
        return (scalar != 0) ? FVector(X / scalar, Y / scalar, Z / scalar) : FVector(0, 0, 0);
    }
    FVector& operator+=(const FVector& rhs) {
        X += rhs.X; Y += rhs.Y; Z += rhs.Z;
        return *this;
    }
    FVector& operator-=(const FVector& rhs) {
        X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z;
        return *this;
    }
    FVector& operator*=(float scalar) {
        X *= scalar; Y *= scalar; Z *= scalar;
        return *this;
    }
    FVector& operator/=(float scalar) {
        if (scalar != 0) { X /= scalar; Y /= scalar; Z /= scalar; }
        return *this;
    }
    bool operator==(const FVector& rhs) const {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
    }
    bool operator!=(const FVector& rhs) const {
        return !(*this == rhs);
    }
};

// Constant Vectors
const FVector ZeroVector(0.0f, 0.0f, 0.0f);
const FVector UpVector(0.0f, 0.0f, 1.0f);