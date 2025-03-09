#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UArrowMeshData.h"
#include "UCamera.h"


class UGizmo : public UPrimitiveComponent
{
public:
    UGizmo(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Gizmo, Position, Rotation, Scale)
    {
        MeshData = &UArrowMeshData::GetInstance();
    }

    void Render(FMatrix view, FMatrix projection) {
        //void Render() {
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
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferX(), MeshData->GetIndexBufferX(), MeshData->GetNumIndex());
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferY(), MeshData->GetIndexBufferY(), MeshData->GetNumIndex());
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferZ(), MeshData->GetIndexBufferZ(), MeshData->GetNumIndex());
    }

private:
    UArrowMeshData* MeshData; // 정적 데이터 참조
};
