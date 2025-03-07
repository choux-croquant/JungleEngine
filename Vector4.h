#pragma once
#include <cmath>

struct FVector4
{
    float X;
    float Y;
    float Z;
    float W;

    FVector4() : X(0), Y(0), Z(0), W(0) {}
    FVector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

    float Dot(const FVector4& rhs) const {
        return X * rhs.X + Y * rhs.Y + Z * rhs.Z + W * rhs.W;
    }

    float Length() const {
        return std::sqrt(X * X + Y * Y + Z * Z + W * W);
    }

    void Normalize() {
        float len = Length();
        if (len > 0) {
            X /= len;
            Y /= len;
            Z /= len;
            W /= len;
        }
    }

    // Operator overloading
    FVector4 operator+(const FVector4& rhs) const {
        return FVector4(X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W);
    }
    FVector4 operator-(const FVector4& rhs) const {
        return FVector4(X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W);
    }
    FVector4 operator*(float scalar) const {
        return FVector4(X * scalar, Y * scalar, Z * scalar, W * scalar);
    }
    FVector4 operator/(float scalar) const {
        return (scalar != 0) ? FVector4(X / scalar, Y / scalar, Z / scalar, W / scalar) : FVector4(0, 0, 0, 0);
    }
    FVector4& operator+=(const FVector4& rhs) {
        X += rhs.X; Y += rhs.Y; Z += rhs.Z; W += rhs.W;
        return *this;
    }
    FVector4& operator-=(const FVector4& rhs) {
        X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; W -= rhs.W;
        return *this;
    }
    FVector4& operator*=(float scalar) {
        X *= scalar; Y *= scalar; Z *= scalar; W *= scalar;
        return *this;
    }
    FVector4& operator/=(float scalar) {
        if (scalar != 0) { X /= scalar; Y /= scalar; Z /= scalar; W /= scalar; }
        return *this;
    }
    bool operator==(const FVector4& rhs) const {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;
    }
    bool operator!=(const FVector4& rhs) const {
        return !(*this == rhs);
    }
};

// Constant Vectors
const FVector4 ZeroVector4(0.0f, 0.0f, 0.0f, 0.0f);

