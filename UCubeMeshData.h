#pragma once
#include "Utils.h"
#include "URenderer.h"
#include "Types.h"

class UCubeMeshData
{
public:
    static UCubeMeshData& GetInstance()
    {
        static UCubeMeshData Instance;
        return Instance;
    }

    ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer; }
    ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
    TArray<FVertexSimple> GetVertices() const { return Vertices; }
    TArray<uint32> GetIndices() const { return Indices; }
    const int32 GetNumIndex() const { return numIndex; }
private:
    UCubeMeshData()
    {
        // ���� ������ �ʱ�ȭ
        Vertices =
        {
            { FVector(-0.5f, -0.5f,  0.5f), FVector4(1.0f, 0.0f, 1.0f, 1.0f) },
            { FVector(0.5f, -0.5f,  0.5f), FVector4(0.0f, 1.0f, 1.0f, 1.0f) },
            { FVector(0.5f,  0.5f,  0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f) },
            { FVector(-0.5f,  0.5f,  0.5f), FVector4(0.0f, 0.0f, 0.0f, 1.0f) },
            { FVector(-0.5f, -0.5f, -0.5f), FVector4(1.0f, 0.0f, 0.0f, 1.0f) },
            { FVector(0.5f, -0.5f, -0.5f), FVector4(0.0f, 1.0f, 0.0f, 1.0f) },
            { FVector(0.5f,  0.5f, -0.5f), FVector4(0.0f, 0.0f, 1.0f, 1.0f) },
            { FVector(-0.5f,  0.5f, -0.5f), FVector4(1.0f, 1.0f, 0.0f, 1.0f) }
        };

        // �ε��� ������ �ʱ�ȭ
        Indices =
        {
            0, 3, 2, 2, 1, 0, // Front
            4, 5, 6, 6, 7, 4, // Back
            0, 4, 7, 7, 3, 0, // Left
            1, 2, 6, 6, 5, 1, // Right
            3, 7, 6, 6, 2, 3, // Top
            0, 1, 5, 5, 4, 0  // Bottom
        };

        // ���� ����
        numIndex = sizeof(Indices) / sizeof(Indices[0]);
        vertexBuffer = URenderer::GetInstance().CreateVertexBuffer(Vertices, 8);
        indexBuffer = URenderer::GetInstance().CreateIndexBuffer(Indices, numIndex);
    }

    ~UCubeMeshData()
    {
        if (vertexBuffer) vertexBuffer->Release();
        if (indexBuffer) indexBuffer->Release();
    }

    TArray<FVertexSimple> Vertices;
    TArray<uint32> Indices;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    uint32 numIndex;
};