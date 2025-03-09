#pragma once
#include "Utils.h"
#include "URenderer.h"
#include "Types.h"
#include "UGeometryGenerator.h"

class UConeMeshData
{
public:
    static UConeMeshData& GetInstance()
    {
        static UConeMeshData Instance;
        return Instance;
    }

    ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer; }
    ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
    const MeshData& GetMeshData() const { return meshData; }
    const int32 GetNumIndex() const { return numIndex; }
private:
    UConeMeshData()
    {
        MeshData coneMesh;

        // CreateSphere 호출
        UGeometryGenerator::CreateCone(1.0f, 2.0f, 8, 8, coneMesh);
        meshData = coneMesh;

        // 버퍼 생성
        numIndex = meshData.Indices.size();
        uint32 numVertex = meshData.Vertices.size();
        vertexBuffer = URenderer::GetInstance().CreateVertexBuffer(meshData.Vertices, numVertex);
        indexBuffer = URenderer::GetInstance().CreateIndexBuffer(meshData.Indices, numIndex);
    }

    ~UConeMeshData()
    {
        if (vertexBuffer) vertexBuffer->Release();
        if (indexBuffer) indexBuffer->Release();
    }

    MeshData meshData;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    uint32 numIndex;
};