#pragma once
#include <cmath>
#include "Vector.h"
#include "Vector4.h"

struct FMatrix
{
    float M[4][4];

    static const FMatrix Identity;

    FMatrix()
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                M[i][j] = 0.0f;
    }

    FMatrix(float values[4][4])
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                M[i][j] = values[i][j];
    }

    FMatrix(const FVector& vec)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                M[i][j] = (i == j) ? 1.0f : 0.0f;

        M[0][3] = vec.X;
        M[1][3] = vec.Y;
        M[2][3] = vec.Z;
    }

    FMatrix(const FVector4& vec)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                M[i][j] = (i == j) ? 1.0f : 0.0f;

        M[0][3] = vec.X;
        M[1][3] = vec.Y;
        M[2][3] = vec.Z;
        M[3][3] = vec.W;
    }

    FMatrix Multiply(const FMatrix& rhs) const
    {
        FMatrix result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.M[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    result.M[i][j] += M[i][k] * rhs.M[k][j];
                }
            }
        }
        return result;
    }

    FVector TransformVector(const FVector& vec) const
    {
        return FVector(
            M[0][0] * vec.X + M[0][1] * vec.Y + M[0][2] * vec.Z + M[0][3],
            M[1][0] * vec.X + M[1][1] * vec.Y + M[1][2] * vec.Z + M[1][3],
            M[2][0] * vec.X + M[2][1] * vec.Y + M[2][2] * vec.Z + M[2][3]
        );
    }

    FMatrix Transpose() const
    {
        FMatrix result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.M[i][j] = M[j][i];
        return result;
    }

    // 모델 변환 행렬 (Translation, Rotation, Scale)
    static FMatrix Translate(const FVector& vec)
    {
        FMatrix result = Identity;
        result.M[0][3] = vec.X;
        result.M[1][3] = vec.Y;
        result.M[2][3] = vec.Z;
        return result;
    }

    static FMatrix Scale(FVector rhs)
    {
        FMatrix result = Identity;
        result.M[0][0] = rhs.X;
        result.M[1][1] = rhs.Y;
        result.M[2][2] = rhs.Z;
        return result;
    }

    static FMatrix Scale(float scale)
    {
        FMatrix result = Identity;
        result.M[0][0] = scale;
        result.M[1][1] = scale;
        result.M[2][2] = scale;
        return result;
    }

    static FMatrix RotateX(float angle)
    {
        FMatrix result = Identity;
        float c = cos(angle);
        float s = sin(angle);
        result.M[1][1] = c;
        result.M[1][2] = -s;
        result.M[2][1] = s;
        result.M[2][2] = c;
        return result;
    }

    static FMatrix RotateY(float angle)
    {
        FMatrix result = Identity;
        float c = cos(angle);
        float s = sin(angle);
        result.M[0][0] = c;
        result.M[0][2] = s;
        result.M[2][0] = -s;
        result.M[2][2] = c;
        return result;
    }

    static FMatrix RotateZ(float angle)
    {
        FMatrix result = Identity;
        float c = cos(angle);
        float s = sin(angle);
        result.M[0][0] = c;
        result.M[0][1] = -s;
        result.M[1][0] = s;
        result.M[1][1] = c;
        return result;
    }

    // 뷰 행렬 (LookAt)
    static FMatrix LookAt(const FVector& eye, const FVector& target, const FVector& up)
    {
        FVector zAxis = (eye - target).Normalize();
        FVector xAxis = up.Cross(zAxis).Normalize();
        FVector yAxis = zAxis.Cross(xAxis);

        FMatrix result = Identity;
        result.M[0][0] = xAxis.X; result.M[0][1] = xAxis.Y; result.M[0][2] = xAxis.Z;
        result.M[1][0] = yAxis.X; result.M[1][1] = yAxis.Y; result.M[1][2] = yAxis.Z;
        result.M[2][0] = zAxis.X; result.M[2][1] = zAxis.Y; result.M[2][2] = zAxis.Z;
        result.M[0][3] = -xAxis.Dot(eye);
        result.M[1][3] = -yAxis.Dot(eye);
        result.M[2][3] = -zAxis.Dot(eye);
        result.M[3][3] = 1.0f;
        return result;
    }

    // 원근 투영 행렬 (Perspective Projection)
    static FMatrix Perspective(float fov, float aspect, float nearZ, float farZ)
    {
        FMatrix result = {};
        float tanHalfFov = tan(fov / 2.0f);
        result.M[0][0] = 1.0f / (aspect * tanHalfFov);
        result.M[1][1] = 1.0f / tanHalfFov;
        result.M[2][2] = -(farZ + nearZ) / (farZ - nearZ);
        result.M[2][3] = -2.0f * farZ * nearZ / (farZ - nearZ);
        result.M[3][2] = -1.0f;
        return result;
    }

    FMatrix operator*(FMatrix rhs) const {
        return this->Multiply(rhs);
    }
};

// Identity matrix definition
const FMatrix FMatrix::Identity = [] {
    FMatrix identity;
    for (int i = 0; i < 4; ++i)
        identity.M[i][i] = 1.0f;
    return identity;
}();
