#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "USphereMeshData.h"
#include "UCamera.h"


class USphereComp : public UPrimitiveComponent
{
public:
    USphereComp(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(Position, Rotation, Scale)
    {
        MeshData = &USphereMeshData::GetInstance();
        meshData = MeshData->GetMeshData();
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
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    USphereMeshData* MeshData; // ���� ������ ����
};
