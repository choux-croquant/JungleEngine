#pragma once
#include "Utils.h"
#include "URenderer.h"
#include "Types.h"

class UArrowMeshData
{
public:
    static UArrowMeshData& GetInstance()
    {
        static UArrowMeshData Instance;
        return Instance;
    }

    ID3D11Buffer* GetVertexBufferX() const { return vertexBufferX; }
    ID3D11Buffer* GetVertexBufferY() const { return vertexBufferY; }
    ID3D11Buffer* GetVertexBufferZ() const { return vertexBufferZ; }
    ID3D11Buffer* GetIndexBufferX() const { return indexBufferX; }
    ID3D11Buffer* GetIndexBufferY() const { return indexBufferY; }
    ID3D11Buffer* GetIndexBufferZ() const { return indexBufferZ; }
    const int32 GetNumIndex() const { return numIndex; }
private:
    UArrowMeshData()
    {
        // X축 화살표 정점 (실린더 + 원뿔)
        FVertexSimple xAxisVertices[] = {
            { { 0.0f, 0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 0.035f, 0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.0f, 0.0f, 0.05f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, -0.035f, 0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, -0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, -0.035f, -0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 0.0f, -0.05f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 0.035f, -0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 

            { { 1.0f, 0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.035f, 0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, 0.05f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 1.0f, -0.035f, 0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 1.0f, -0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, 
            { { 1.0f, -0.035f, -0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, -0.05f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.035f, -0.035f }, { 1.0f, 0.0f, 0.0f, 1.0f } },

            // 원뿔 정점
            { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 원뿔 밑면 중심
            { { 1.2f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 원뿔 끝점
            { { 1.0f, 0.1f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 원뿔 밑면 원주
            { { 1.0f, -0.1f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, 0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, -0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
        };

        // X축 화살표 인덱스 (실린더 + 원뿔)
        uint32 xAxisIndices[] = {
            // 실린더 측면
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // 시작점과 끝점 연결

            // 실린더 상단 원
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // 실린더 하단 원
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // 원뿔 밑면
            18, 20, 19, 21, 
            // 원뿔
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // Y축 화살표 정점 (실린더 + 원뿔)
        FVertexSimple yAxisVertices[] = {
            // 실린더 정점 (원주를 따라 8개의 정점)
            { { 0.05f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.035f, 0.0f, 0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 0.0f, 0.05f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.035f, 0.0f, 0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.05f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.035f, 0.0f, -0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.0f, 0.0f, -0.05f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.035f, 0.0f, -0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } },

            { { 0.05f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.035f, 1.0f, 0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 1.0f, 0.05f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.035f, 1.0f, 0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { -0.05f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { -0.035f, 1.0f, -0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, 
            { { 0.0f, 1.0f, -0.05f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.035f, 1.0f, -0.035f }, { 0.0f, 1.0f, 0.0f, 1.0f } },

            // 원뿔 정점
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 원뿔 밑면 중심
            { { 0.0f, 1.2f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 원뿔 끝점
            { { 0.1f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 원뿔 밑면 원주
            { { -0.1f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.0f, 1.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.0f, 1.0f, -0.1f }, { 0.0f, 1.0f, 0.0f, 1.0f } }
        };

        // Y축 화살표 인덱스 (실린더 + 원뿔)
        uint32 yAxisIndices[] = {
            // 실린더 측면
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // 시작점과 끝점 연결

            // 실린더 상단 원
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // 실린더 하단 원
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // 원뿔 밑면
            18, 20, 19, 21,

            // 원뿔
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // Z축 화살표 정점 (실린더 + 원뿔)
        FVertexSimple zAxisVertices[] = {
            // 실린더 정점 (원주를 따라 8개의 정점)
            { { 0.05f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.035f, 0.035f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.0f, 0.05f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.035f, 0.035f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.05f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.035f, -0.035f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, -0.05f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.035f, -0.035f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 

            { { 0.05f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.035f, 0.035f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, 0.05f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.035f, 0.035f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -0.05f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { -0.035f, -0.035f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.0f, -0.05f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, 
            { { 0.035f, -0.035f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

            // 원뿔 정점
            { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 원뿔 밑면 중심
            { { 0.0f, 0.0f, 1.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 원뿔 끝점
            { { 0.1f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 원뿔 밑면 원주
            { { -0.1f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, 0.1f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, -0.1f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };

        // Z축 화살표 인덱스 (실린더 + 원뿔)
        uint32 zAxisIndices[] = {
            // 실린더 측면
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // 시작점과 끝점 연결

            // 실린더 상단 원
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // 실린더 하단 원
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // 원뿔 밑면
            18, 20, 19, 21,

            // 원뿔
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // 버퍼 생성
        numIndex = sizeof(xAxisIndices) / sizeof(xAxisIndices[0]);
        vertexBufferX = URenderer::GetInstance().CreateVertexBuffer(xAxisVertices, 14);
        vertexBufferY = URenderer::GetInstance().CreateVertexBuffer(yAxisVertices, 14);
        vertexBufferZ = URenderer::GetInstance().CreateVertexBuffer(zAxisVertices, 14);
        indexBufferX = URenderer::GetInstance().CreateIndexBuffer(xAxisIndices, numIndex);
        indexBufferY = URenderer::GetInstance().CreateIndexBuffer(yAxisIndices, numIndex);
        indexBufferZ = URenderer::GetInstance().CreateIndexBuffer(zAxisIndices, numIndex);
    }

    ~UArrowMeshData()
    {
        if (vertexBufferX) vertexBufferX->Release();
        if (vertexBufferY) vertexBufferY->Release();
        if (vertexBufferZ) vertexBufferZ->Release();
        if (indexBufferX) indexBufferX->Release();
        if (indexBufferY) indexBufferY->Release();
        if (indexBufferZ) indexBufferZ->Release();
    }

    ID3D11Buffer* vertexBufferX;
    ID3D11Buffer* vertexBufferY;
    ID3D11Buffer* vertexBufferZ;
    ID3D11Buffer* indexBufferX;
    ID3D11Buffer* indexBufferY;
    ID3D11Buffer* indexBufferZ;
    uint32 numIndex;
};