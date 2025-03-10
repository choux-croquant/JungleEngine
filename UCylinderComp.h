#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCylinderMeshData.h"
#include "UCamera.h"


class UCylinderComp : public UPrimitiveComponent
{
public:
    UCylinderComp(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Cylinder, Position, Rotation, Scale)
    {
        MeshData = &UCylinderMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
    }

    void Render(FMatrix view, FMatrix projection) override  {
        FConstants constantData = {};

        FMatrix model = GetWorldTransform();

        // MVP 행렬 계산
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // 렌더링 데이터 업데이트 및 그리기
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCylinderMeshData* MeshData; // 정적 데이터 참조
};
