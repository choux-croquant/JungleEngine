#pragma once
#include "Utils.h"
#include "URenderer.h"
#include "Types.h"
#include "UGeometryGenerator.h"

class USphereMeshData
{
public:
    static USphereMeshData& GetInstance()
    {
        static USphereMeshData Instance;
        return Instance;
    }

    ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer; }
    ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
    const int32 GetNumIndex() const { return numIndex; }
private:
    USphereMeshData()
    {
        MeshData sphereMesh;

        // CreateSphere 호출
        UGeometryGenerator::CreateSphere(1.0f, 12, 12, sphereMesh);
        meshData = sphereMesh;

        // 버퍼 생성
        numIndex = meshData.Indices.size();
        uint32 numVertex = meshData.Vertices.size();
        vertexBuffer = URenderer::GetInstance().CreateVertexBuffer(meshData.Vertices, numVertex);
        indexBuffer = URenderer::GetInstance().CreateIndexBuffer(meshData.Indices, numIndex);
    }

    ~USphereMeshData()
    {
        if (vertexBuffer) vertexBuffer->Release();
        if (indexBuffer) indexBuffer->Release();
    }

    MeshData meshData;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    uint32 numIndex;
};