#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UConeMeshData.h"
#include "UCamera.h"


class UConeComp : public UPrimitiveComponent
{
public:
    UConeComp(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Cone, Position, Rotation, Scale)
    {
        MeshData = &UConeMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
    }
    UConeComp(FVector Position, FVector Rotation, FVector Scale, FVector4 Color, bool flag)
        : UPrimitiveComponent(EPrimitiveType::Cone, Position, Rotation, Scale)
    {
        MeshData = &UConeMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
        mColor = Color;
        mFlag = flag;
    }
    void Render(FMatrix view, FMatrix projection) override {
        //void Render() {
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
    UConeMeshData* MeshData; // ���� ������ ����
    FVector4 mColor;
    bool mFlag = false;
};
