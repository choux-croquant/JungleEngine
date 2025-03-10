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
    UCylinderComp(FVector Position, FVector Rotation, FVector Scale, FVector4 Color, bool flag)
        : UPrimitiveComponent(EPrimitiveType::Cylinder, Position, Rotation, Scale)
    {
        MeshData = &UCylinderMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
        mColor = Color;
        mFlag = flag;
    }

    void Render(FMatrix view, FMatrix projection) override  {
        FConstants constantData = {};

        FMatrix model = GetWorldTransform();

        // MVP ��� ���
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // ������ ������ ������Ʈ �� �׸���
        URenderer::GetInstance().UpdateConstant(constantData);
        if (mFlag) {
            URenderer::GetInstance().UpdateConstant2(mColor);
            URenderer::GetInstance().PreparePixelShader2();
        }
        else {
            URenderer::GetInstance().PreparePixelShader();
        }
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCylinderMeshData* MeshData; // ���� ������ ����
    FVector4 mColor;
    bool mFlag = false;
};
