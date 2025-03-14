#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCubeMeshData.h"
#include "UCamera.h"


class UCubeComp : public UPrimitiveComponent
{
public:
    UCubeComp(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Cube, Position, Rotation, Scale)
    {
        MeshData = &UCubeMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
    }

    void Render(FMatrix view, FMatrix projection) override {
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
        constantData.bClicked = bIsClicked ? 1 : 0;

        // 렌더링 데이터 업데이트 및 그리기
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().PreparePixelShader();
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCubeMeshData* MeshData; // 정적 데이터 참조
};
