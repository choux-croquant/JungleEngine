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

        FMatrix scaleMatrix = FMatrix::Scale(GetWorldScale3D());

        // Rotation ��� (�� �ະ ȸ�� ����)
        FMatrix rotationMatrix =
            FMatrix::RotateZ(GetWorldRotation().Z) *
            FMatrix::RotateY(GetWorldRotation().Y) *
            FMatrix::RotateX(GetWorldRotation().X);

        // Translation(�̵�) ���
        FMatrix translationMatrix = FMatrix::Identity.Translate(GetWorldLocation());

        // ModelMatrix = Scale * Rotation * Translation
        FMatrix model = translationMatrix * rotationMatrix * scaleMatrix;

        // MVP ��� ���
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // ������ ������ ������Ʈ �� �׸���
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCylinderMeshData* MeshData; // ���� ������ ����
};
