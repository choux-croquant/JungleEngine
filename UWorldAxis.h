#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCubeMeshData.h"
#include "UCamera.h"


class UWorldAxis : public UPrimitiveComponent
{
public:
    UWorldAxis(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Axis, Position, Rotation, Scale)
    {
        TArray<FVertexSimple> axisVertices = {
            // X축 (빨간색)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f) }, // 시작점
            { FVector(1000.0f, 0.0f, 0.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f) }, // 끝점

            // Y축 (초록색)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f) }, // 시작점
            { FVector(0.0f, 1000.0f, 0.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f) }, // 끝점

            // Z축 (파란색)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f) }, // 시작점
            { FVector(0.0f, 0.0f, 1000.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f) }  // 끝점
        };

        TArray<uint32> axisIndices = {
            0, 1, // X축
            2, 3, // Y축
            4, 5  // Z축
        };

        vertexBuffer = URenderer::GetInstance().CreateVertexBuffer(axisVertices, 6);
        indexBuffer = URenderer::GetInstance().CreateIndexBuffer(axisIndices, 6);
    }

    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;

    void Render(FMatrix view, FMatrix projection) {
        FConstants constantData = {};

        // Scale 행렬
        FMatrix scaleMatrix = FMatrix::Scale(RelativeScale3D);

        // Rotation 행렬 (각 축별 회전 적용)
        FMatrix rotationMatrix =
            FMatrix::RotateZ(RelativeRotation.Z) *
            FMatrix::RotateY(RelativeRotation.Y) *
            FMatrix::RotateX(RelativeRotation.X);

        // Translation(이동) 행렬
        FMatrix translationMatrix = FMatrix::Identity.Translate(RelativeLocation);

        // ModelMatrix = Scale * Rotation * Translation
        FMatrix model = translationMatrix * rotationMatrix * scaleMatrix;

        // MVP 행렬 계산
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // 렌더링 데이터 업데이트 및 그리기
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderLine(vertexBuffer, indexBuffer, 6);
    }

private:
    UCubeMeshData* MeshData; // 정적 데이터 참조
};
