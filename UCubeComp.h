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
        FMatrix view = FMatrix::LookAt(
            FVector(0.0f, 2.0f, -10.0f), // ī�޶� ��ġ
            FVector(0.0f, 0.0f, 0.0f),   // �ٶ󺸴� ����
            FVector(0.0f, 1.0f, 0.0f)    // �� ����
        );

        // ProjectionMatrix ����
        FMatrix projection = FMatrix::Perspective(
            0.785398f,       // FOV (45��)
            (float)1024 / 1024, // ȭ�� ����
            1.0f,            // ����� Ŭ���� ���
            100.0f           // �� Ŭ���� ���
        );

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
