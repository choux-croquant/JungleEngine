#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCubeMeshData.h"
#include "UCamera.h"

extern UCamera mainCamera;

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

        // ViewMatrix ����
        FMatrix view = mainCamera.viewMatrix;

        // ProjectionMatrix ����
        FMatrix projection = mainCamera.projectionMatrix;

        // MVP ��� ���
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // ������ ������ ������Ʈ �� �׸���
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive(MeshData->GetVertexBuffer(), MeshData->GetIndexBuffer(), MeshData->GetNumIndex());
    }

private:
    UCubeMeshData* MeshData; // ���� ������ ����
};
