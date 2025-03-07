#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCubeMeshData.h"

class UCubeComp : public UPrimitiveComponent
{
public:
    UCubeComp(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(Position, Rotation, Scale)
    {
        MeshData = &UCubeMeshData::GetInstance();
    }

    void Render() {
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

        // ViewMatrix 설정
        FMatrix view = FMatrix::LookAt(
            FVector(0.0f, 2.0f, -10.0f), // 카메라 위치
            FVector(0.0f, 0.0f, 0.0f),   // 바라보는 지점
            FVector(0.0f, 1.0f, 0.0f)    // 업 벡터
        );

        // ProjectionMatrix 설정
        FMatrix projection = FMatrix::Perspective(
            0.785398f,       // FOV (45도)
            (float)1024 / 1024, // 화면 비율
            1.0f,            // 가까운 클리핑 평면
            100.0f           // 먼 클리핑 평면
        );

        // MVP 행렬 계산
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // 렌더링 데이터 업데이트 및 그리기
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCubeMeshData* MeshData; // 정적 데이터 참조
};
