#pragma once
#include <cmath>
#include "Vector.h"
#include "Vector4.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "Types.h"

static const float PI = 3.14159265358979323846f;

static float RadtoDeg(float a) {
    return a * 180.0f / PI;
}

static float DegtoRad(float a) {
    return a * PI / 180.0f;
}

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
    FVector4 TransformVector(const FVector4& vec) const
    {
        return FVector4(
            M[0][0] * vec.X + M[0][1] * vec.Y + M[0][2] * vec.Z + M[0][3]*vec.W,
            M[1][0] * vec.X + M[1][1] * vec.Y + M[1][2] * vec.Z + M[1][3]*vec.W,
            M[2][0] * vec.X + M[2][1] * vec.Y + M[2][2] * vec.Z + M[2][3]*vec.W,
            M[3][0] * vec.X + M[3][1] * vec.Y + M[3][2] * vec.Z + M[3][3]*vec.W

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
        result.M[1][2] = s;  // 부호 반전
        result.M[2][1] = -s; // 부호 반전
        result.M[2][2] = c;
        return result;
    }

    static FMatrix RotateY(float angle)
    {
        FMatrix result = Identity;
        float c = cos(angle);
        float s = sin(angle);
        result.M[0][0] = c;
        result.M[0][2] = s; // 부호 반전
        result.M[2][0] = -s;  // 부호 반전
        result.M[2][2] = c;
        return result;
    }

    static FMatrix RotateZ(float angle)
    {
        FMatrix result = Identity;
        float c = cos(angle);
        float s = sin(angle);
        result.M[0][0] = c;
        result.M[0][1] = s;  // 부호 반전
        result.M[1][0] = -s; // 부호 반전
        result.M[1][1] = c;
        return result;
    }

    static FMatrix Rotate(const FVector& axis, float angle)
    {
        // 회전축을 정규화
        FVector nAxis = axis;
        nAxis.Normalize();

        float cosA = cos(angle);
        float sinA = sin(angle);

        float oneMinusCosA = 1.0f - cosA;

        // 미리 계산
        float xx = nAxis.X * nAxis.X;
        float yy = nAxis.Y * nAxis.Y;
        float zz = nAxis.Z * nAxis.Z;
        float xy = nAxis.X * nAxis.Y;
        float xz = nAxis.X * nAxis.Z;
        float yz = nAxis.Y * nAxis.Z;

        FMatrix result = Identity;
        result.M[0][0] = cosA + xx * oneMinusCosA; result.M[0][1] = xy * oneMinusCosA + nAxis.Z * sinA; result.M[0][2] = xz * oneMinusCosA - nAxis.Y * sinA;
        result.M[1][0] = xy * oneMinusCosA - nAxis.Z * sinA; result.M[1][1] = cosA + yy * oneMinusCosA; result.M[1][2] = yz * oneMinusCosA + nAxis.X * sinA;
        result.M[2][0] = xz * oneMinusCosA + nAxis.Y * sinA; result.M[2][1] = yz * oneMinusCosA - nAxis.X * sinA; result.M[2][2] = cosA + zz * oneMinusCosA;
        return result;
    }

    // 뷰 행렬 (LookAt)
    static FMatrix LookAt(const FVector& eye, const FVector& target, const FVector& up)
    {
        FMatrix result = LookFrom(eye, target-eye, up);

        return result;
    }

    static FMatrix LookFrom(const FVector& eye, const FVector& look, const FVector& up)
    {
        FVector zAxis = const_cast<FVector&>(look).Normalize();
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
        result.M[2][2] = farZ / (farZ - nearZ);
        result.M[2][3] = -farZ * nearZ / (farZ - nearZ);
        result.M[3][2] = 1.0f;
        return result;
    }

    static FMatrix Orthogonal(float left, float right, float bottom, float top, float nearZ, float farZ) {
        FMatrix ortho = Identity;
        ortho.M[0][0] = 2.0f / (right - left);
        ortho.M[1][1] = 2.0f / (top - bottom);
        ortho.M[2][2] = 1.0f / (farZ - nearZ);
        ortho.M[0][3] = -(right + left) / (right - left);
        ortho.M[1][3] = -(top + bottom) / (top - bottom);
        ortho.M[2][3] = -nearZ / (farZ - nearZ);
        ortho.M[3][3] = 1.0f;

        return ortho;
    }

    FMatrix operator*(FMatrix rhs) const {
        return this->Multiply(rhs);
    }

    FVector operator*(FVector& a) const {
        FVector result = FVector();
        float x = M[0][0] * a.X + M[0][1] * a.Y + M[0][2] * a.Z + M[0][3];
        float y = M[1][0] * a.X + M[1][1] * a.Y + M[1][2] * a.Z + M[1][3];
        float z = M[2][0] * a.X + M[2][1] * a.Y + M[2][2] * a.Z + M[2][3];
        return FVector(x, y, z);
    }

    FString PrintMatrix()
    {
        FString result = "";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result += std::to_string(M[i][j]) + ", ";
            }
            result += "\n";
        }
        return result;
    }

    FMatrix InverseGaussJordan() const {
        FMatrix augmented = *this; // 원본 행렬 복사
        FMatrix identity = FMatrix::Identity; // 단위 행렬 생성

        for (int col = 0; col < 4; col++) {
            // 1. 현재 열(col)에서 절댓값이 가장 큰 피벗(pivot) 찾기 (안정성 향상)
            int pivotRow = col;
            for (int row = col + 1; row < 4; row++) {
                if (fabs(augmented.M[row][col]) > fabs(augmented.M[pivotRow][col])) {
                    pivotRow = row;
                }
            }

            // 2. 행 교환 (Pivoting)
            if (fabs(augmented.M[pivotRow][col]) < 1e-6f) {
                std::cerr << "Matrix is singular and cannot be inverted (Gauss-Jordan)." << std::endl;
                return FMatrix::Identity; // 역행렬 없음 (단위 행렬 반환)
            }
            std::swap(augmented.M[col], augmented.M[pivotRow]);            
            std::swap(identity.M[col], identity.M[pivotRow]);

            // 3. 현재 열(col)의 피벗을 1로 만들기
            float pivot = augmented.M[col][col];
            for (int j = 0; j < 4; j++) {
                augmented.M[col][j] /= pivot;
                identity.M[col][j] /= pivot;
            }

            // 4. 현재 열(col)을 제외한 나머지 행(row)의 해당 열(col) 값을 0으로 만들기
            for (int row = 0; row < 4; row++) {
                if (row == col) continue; // 현재 행은 스킵
                float factor = augmented.M[row][col];
                for (int j = 0; j < 4; j++) {
                    augmented.M[row][j] -= factor * augmented.M[col][j];
                    identity.M[row][j] -= factor * identity.M[col][j];
                }
            }
        }

        return identity; // 변환된 단위 행렬이 역행렬이 됨
    }


    FMatrix Inverse() const {
        FMatrix result;
        float det = Determinant();
        if (fabs(det) < 1e-6f) { // 행렬식이 0이면 역행렬이 없음
            std::cerr << "Matrix is singular and cannot be inverted." << std::endl;
            return result; // 기본 행렬 반환 (0으로 채워짐)
        }

        float invDet = 1.0f / det;
        FMatrix adj = Adjugate(); // 수반행렬 계산

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.M[i][j] = adj.M[i][j] * invDet;
            }
        }
        return result;
    }

    float Determinant() const {
        float det = 0.0f;
        for (int i = 0; i < 4; ++i) {
            det += (i % 2 == 0 ? 1.0f : -1.0f) * M[0][i] * Cofactor(0, i);
        }
        return det;
    }

    float Cofactor(int row, int col) const {
        float subMatrix[3][3];
        int subi = 0;
        for (int i = 0; i < 4; i++) {
            if (i == row) continue;
            int subj = 0;
            for (int j = 0; j < 4; j++) {
                if (j == col) continue;
                subMatrix[subi][subj] = M[i][j];
                subj++;
            }
            subi++;
        }

        float det3x3 = subMatrix[0][0] * (subMatrix[1][1] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][1])
            - subMatrix[0][1] * (subMatrix[1][0] * subMatrix[2][2] - subMatrix[1][2] * subMatrix[2][0])
            + subMatrix[0][2] * (subMatrix[1][0] * subMatrix[2][1] - subMatrix[1][1] * subMatrix[2][0]);

        return ((row + col) % 2 == 0 ? 1.0f : -1.0f) * det3x3;
    }

    FMatrix Adjugate() const {
        FMatrix adj;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                adj.M[i][j] = Cofactor(j, i); // 전치하여 저장
            }
        }
        return adj;
    }

};
