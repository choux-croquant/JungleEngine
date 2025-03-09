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

        // Scale ���
        FMatrix scaleMatrix = FMatrix::Scale(RelativeScale3D);

        // Rotation ��� (�� �ະ ȸ�� ����)
        FMatrix rotationMatrix =
            FMatrix::RotateZ(RelativeRotation.Z) *
            FMatrix::RotateY(RelativeRotation.Y) *
            FMatrix::RotateX(RelativeRotation.X);

        // Translation(�̵�) ���
        FMatrix translationMatrix = FMatrix::Identity.Translate(RelativeLocation);

        // ModelMatrix = Scale * Rotation * Translation
        FMatrix model = translationMatrix * rotationMatrix * scaleMatrix;

        // MVP ��� ���
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // ������ ������ ������Ʈ �� �׸���
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferX(), MeshData->GetIndexBufferX(), MeshData->GetNumIndex());
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferY(), MeshData->GetIndexBufferY(), MeshData->GetNumIndex());
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBufferZ(), MeshData->GetIndexBufferZ(), MeshData->GetNumIndex());
    }

private:
    UArrowMeshData* MeshData; // ���� ������ ����
};
