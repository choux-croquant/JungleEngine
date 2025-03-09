#pragma once
#include "UPrimitiveComponent.h"
#include "Utils.h"
#include "URenderer.h"
#include "UCubeMeshData.h"
#include "UCamera.h"


class UWorldAxis : public UPrimitiveComponent
{
public:
    UWorldAxis(FVector Position, FVector Rotation, FVector Scale)
        : UPrimitiveComponent(EPrimitiveType::Axis, Position, Rotation, Scale)
    {
        TArray<FVertexSimple> axisVertices = {
            // X�� (������)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f) }, // ������
            { FVector(1000.0f, 0.0f, 0.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f) }, // ����

            // Y�� (�ʷϻ�)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f) }, // ������
            { FVector(0.0f, 1000.0f, 0.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f) }, // ����

            // Z�� (�Ķ���)
            { FVector(0.0f, 0.0f, 0.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f) }, // ������
            { FVector(0.0f, 0.0f, 1000.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f) }  // ����
        };

        TArray<uint32> axisIndices = {
            0, 1, // X��
            2, 3, // Y��
            4, 5  // Z��
        };

        vertexBuffer = URenderer::GetInstance().CreateVertexBuffer(axisVertices, 6);
        indexBuffer = URenderer::GetInstance().CreateIndexBuffer(axisIndices, 6);
    }

    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;

    void Render(FMatrix view, FMatrix projection) {
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
        URenderer::GetInstance().RenderLine(vertexBuffer, indexBuffer, 6);
    }

private:
    UCubeMeshData* MeshData; // ���� ������ ����
};
