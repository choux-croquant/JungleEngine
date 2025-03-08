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
        // X�� ȭ��ǥ ���� (�Ǹ��� + ����)
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

            // ���� ����
            { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // ���� �ظ� �߽�
            { { 1.2f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // ���� ����
            { { 1.0f, 0.1f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // ���� �ظ� ����
            { { 1.0f, -0.1f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, 0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f, -0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
        };

        // X�� ȭ��ǥ �ε��� (�Ǹ��� + ����)
        uint32 xAxisIndices[] = {
            // �Ǹ��� ����
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // �������� ���� ����

            // �Ǹ��� ��� ��
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // �Ǹ��� �ϴ� ��
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // ���� �ظ�
            18, 20, 19, 21, 
            // ����
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // Y�� ȭ��ǥ ���� (�Ǹ��� + ����)
        FVertexSimple yAxisVertices[] = {
            // �Ǹ��� ���� (���ָ� ���� 8���� ����)
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

            // ���� ����
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // ���� �ظ� �߽�
            { { 0.0f, 1.2f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // ���� ����
            { { 0.1f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // ���� �ظ� ����
            { { -0.1f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.0f, 1.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 0.0f, 1.0f, -0.1f }, { 0.0f, 1.0f, 0.0f, 1.0f } }
        };

        // Y�� ȭ��ǥ �ε��� (�Ǹ��� + ����)
        uint32 yAxisIndices[] = {
            // �Ǹ��� ����
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // �������� ���� ����

            // �Ǹ��� ��� ��
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // �Ǹ��� �ϴ� ��
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // ���� �ظ�
            18, 20, 19, 21,

            // ����
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // Z�� ȭ��ǥ ���� (�Ǹ��� + ����)
        FVertexSimple zAxisVertices[] = {
            // �Ǹ��� ���� (���ָ� ���� 8���� ����)
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

            // ���� ����
            { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // ���� �ظ� �߽�
            { { 0.0f, 0.0f, 1.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // ���� ����
            { { 0.1f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // ���� �ظ� ����
            { { -0.1f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, 0.1f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, -0.1f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };

        // Z�� ȭ��ǥ �ε��� (�Ǹ��� + ����)
        uint32 zAxisIndices[] = {
            // �Ǹ��� ����
            0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15,
            0, 8, // �������� ���� ����

            // �Ǹ��� ��� ��
            8, 9, 10, 11, 12, 13, 14, 15, 8,

            // �Ǹ��� �ϴ� ��
            0, 1, 2, 3, 4, 5, 6, 7, 0,

            // ���� �ظ�
            18, 20, 19, 21,

            // ����
            16, 17, 18,
            16, 17, 19,
            16, 17, 20,
            16, 17, 21
        };

        // ���� ����
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