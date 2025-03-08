#include "UGeometryGenerator.h"
#include <cmath>

void UGeometryGenerator::CreateSphere(int32 radius, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 정점과 인덱스 배열 초기화
    meshData.Vertices.clear();
    meshData.Indices.clear();

    // 상단 극점 추가
    FVertexSimple topVertex;
    topVertex.Position = FVector(0.0f, radius, 0.0f);
    topVertex.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    meshData.Vertices.push_back(topVertex);

    // 각 스택(위도)과 슬라이스(경도)를 순회하며 정점 생성
    for (uint32 stack = 1; stack <= stackCount - 1; ++stack)
    {
        float phi = PI * stack / stackCount; // 위도 (0 ~ PI)
        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2 * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

            // 정점 위치 계산
            FVertexSimple vertex;
            vertex.Position.X = radius * sinf(phi) * cosf(theta);
            vertex.Position.Y = radius * cosf(phi);
            vertex.Position.Z = radius * sinf(phi) * sinf(theta);

            // 정점 색상 설정
            vertex.Color = FVector4(
                static_cast<float>(stack) / stackCount,
                static_cast<float>(slice) / sliceCount,
                1.0f - static_cast<float>(stack) / stackCount,
                1.0f
            );

            // 정점 추가
            meshData.Vertices.push_back(vertex);
        }
    }

    // 하단 극점 추가
    FVertexSimple bottomVertex;
    bottomVertex.Position = FVector(0.0f, -radius, 0.0f);
    bottomVertex.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
    meshData.Vertices.push_back(bottomVertex);

    // 상단 극점과 첫 번째 스택 연결
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        meshData.Indices.push_back(0); // 상단 극점
        meshData.Indices.push_back(slice + 1);
        meshData.Indices.push_back(slice); 
    }

    // 중간 스택들 연결
    for (uint32 stack = 0; stack < stackCount - 2; ++stack)
    {
        uint32 baseIndex = 1 + stack * (sliceCount + 1); // 현재 스택의 시작 인덱스
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            // 첫 번째 삼각형
            meshData.Indices.push_back(baseIndex + slice);
            meshData.Indices.push_back(baseIndex + slice + 1);
            meshData.Indices.push_back(baseIndex + slice + (sliceCount + 1));

            // 두 번째 삼각형
            meshData.Indices.push_back(baseIndex + slice + 1);
            meshData.Indices.push_back(baseIndex + slice + (sliceCount + 1) + 1);
            meshData.Indices.push_back(baseIndex + slice + (sliceCount + 1));
        }
    }

    // 하단 극점과 마지막 스택 연결
    uint32 bottomIndex = meshData.Vertices.size() - 1; // 하단 극점 인덱스
    uint32 lastStackBaseIndex = bottomIndex - (sliceCount + 1); // 마지막 스택의 시작 인덱스
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        meshData.Indices.push_back(bottomIndex); // 하단 극점
        meshData.Indices.push_back(lastStackBaseIndex + slice);
        meshData.Indices.push_back(lastStackBaseIndex + slice + 1);
    }
}

void UGeometryGenerator::CreateCylinder(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 정점과 인덱스 배열 초기화
    meshData.Vertices.clear();
    meshData.Indices.clear();

    float stackHeight = height / stackCount; // 스택 높이
    float radiusStep = (tRadius - bRadius) / stackCount; // 반지름 변화량

    // 정점 생성
    for (uint32 stack = 0; stack <= stackCount; ++stack)
    {
        float y = -0.5f * height + stack * stackHeight; // y 좌표
        float r = bRadius + stack * radiusStep; // 반지름

        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

            // 정점 위치 계산
            FVertexSimple vertex;
            vertex.Position.X = r * cosf(theta);
            vertex.Position.Y = y;
            vertex.Position.Z = r * sinf(theta);

            // 정점 색상 설정
            vertex.Color = FVector4(
                static_cast<float>(stack) / stackCount,
                static_cast<float>(slice) / sliceCount,
                1.0f - static_cast<float>(stack) / stackCount,
                1.0f
            );

            // 정점 추가
            meshData.Vertices.push_back(vertex);
        }
    }

    // 인덱스 버퍼 생성
    for (uint32 stack = 0; stack < stackCount; ++stack)
    {
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            uint32 baseIndex = stack * (sliceCount + 1) + slice;

            // 첫 번째 삼각형
            meshData.Indices.push_back(baseIndex);
            meshData.Indices.push_back(baseIndex + 1);
            meshData.Indices.push_back(baseIndex + (sliceCount + 1));

            // 두 번째 삼각형
            meshData.Indices.push_back(baseIndex + 1);
            meshData.Indices.push_back(baseIndex + (sliceCount + 1) + 1);
            meshData.Indices.push_back(baseIndex + (sliceCount + 1));
        }
    }
    CreateCylinderTop(bRadius, tRadius, height, sliceCount, stackCount, meshData);
    CreateCylinderBottom(bRadius, tRadius, height, sliceCount, stackCount, meshData);
}

void UGeometryGenerator::CreateCylinderTop(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 윗면의 중심 정점 추가
    FVertexSimple topCenter;
    topCenter.Position = FVector(0.0f, height / 2.0f, 0.0f); // 윗면의 중심
    topCenter.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
    uint32 centerIndex = meshData.Vertices.size(); // 중심 정점의 인덱스 저장
    meshData.Vertices.push_back(topCenter);

    // 윗면의 원주 정점 추가
    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

        FVertexSimple vertex;
        vertex.Position.X = tRadius * cosf(theta); // X 좌표
        vertex.Position.Y = height / 2.0f;        // Y 좌표 (윗면의 높이)
        vertex.Position.Z = tRadius * sinf(theta); // Z 좌표

        // 정점 색상 설정
        vertex.Color = FVector4(
            static_cast<float>(slice) / sliceCount,
            1.0f - static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f
        );

        // 정점 추가
        meshData.Vertices.push_back(vertex);
    }

    // 윗면의 삼각형 구성
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        meshData.Indices.push_back(centerIndex); // 중심 정점
        meshData.Indices.push_back(centerIndex + slice + 1);
        meshData.Indices.push_back(centerIndex + (slice + 1) % sliceCount + 1); // 슬라이스의 끝에서 시작점으로 연결
    }
}

void UGeometryGenerator::CreateCylinderBottom(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 아랫면의 중심 정점 추가
    FVertexSimple bottomCenter;
    bottomCenter.Position = FVector(0.0f, -height / 2.0f, 0.0f); // 아랫면의 중심
    bottomCenter.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
    uint32 centerIndex = meshData.Vertices.size(); // 중심 정점의 인덱스 저장
    meshData.Vertices.push_back(bottomCenter);

    // 아랫면의 원주 정점 추가
    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

        FVertexSimple vertex;
        vertex.Position.X = bRadius * cosf(theta); // X 좌표
        vertex.Position.Y = -height / 2.0f;       // Y 좌표 (아랫면의 높이)
        vertex.Position.Z = bRadius * sinf(theta); // Z 좌표

        // 정점 색상 설정
        vertex.Color = FVector4(
            static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / sliceCount,
            1.0f
        );

        // 정점 추가
        meshData.Vertices.push_back(vertex);
    }

    // 아랫면의 삼각형 구성
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        meshData.Indices.push_back(centerIndex); // 중심 정점
        meshData.Indices.push_back(centerIndex + (slice + 1) % sliceCount + 1); // 슬라이스의 끝에서 시작점으로 연결
        meshData.Indices.push_back(centerIndex + slice + 1);
    }
}

void UGeometryGenerator::CreateCone(float bottomRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 정점과 인덱스 배열 초기화
    meshData.Vertices.clear();
    meshData.Indices.clear();

    // 밑면의 중심 정점 추가
    FVertexSimple bottomCenter;
    bottomCenter.Position = FVector(0.0f, -height / 2.0f, 0.0f); // 밑면의 중심
    bottomCenter.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
    uint32 centerIndex = meshData.Vertices.size(); // 중심 정점의 인덱스 저장
    meshData.Vertices.push_back(bottomCenter);

    // 밑면의 원주 정점 추가
    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

        FVertexSimple vertex;
        vertex.Position.X = bottomRadius * cosf(theta); // X 좌표
        vertex.Position.Y = -height / 2.0f;            // Y 좌표 (밑면의 높이)
        vertex.Position.Z = bottomRadius * sinf(theta); // Z 좌표

        // 정점 색상 설정
        vertex.Color = FVector4(
            static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / sliceCount,
            1.0f
        );

        // 정점 추가
        meshData.Vertices.push_back(vertex);
    }

    // 밑면의 삼각형 구성
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        meshData.Indices.push_back(centerIndex); // 중심 정점
        meshData.Indices.push_back(centerIndex + (slice + 1) % sliceCount + 1); // 슬라이스의 끝에서 시작점으로 연결
        meshData.Indices.push_back(centerIndex + slice + 1);
    }

    // 원뿔의 꼭짓점 추가
    FVertexSimple topVertex;
    topVertex.Position = FVector(0.0f, height / 2.0f, 0.0f); // 원뿔의 꼭짓점
    topVertex.Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 흰색
    uint32 topIndex = meshData.Vertices.size(); // 꼭짓점의 인덱스 저장
    meshData.Vertices.push_back(topVertex);

    // 옆면의 정점 추가
    for (uint32 stack = 0; stack <= stackCount; ++stack)
    {
        float y = -height / 2.0f + (height * stack / stackCount); // y 좌표
        float r = bottomRadius * (1.0f - static_cast<float>(stack) / stackCount); // 반지름 (원뿔의 꼭짓점으로 갈수록 0)

        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / sliceCount; // 경도 (0 ~ 2PI)

            FVertexSimple vertex;
            vertex.Position.X = r * cosf(theta); // X 좌표
            vertex.Position.Y = y;              // Y 좌표
            vertex.Position.Z = r * sinf(theta); // Z 좌표

            // 정점 색상 설정
            vertex.Color = FVector4(
                static_cast<float>(stack) / stackCount,
                static_cast<float>(slice) / sliceCount,
                1.0f - static_cast<float>(stack) / stackCount,
                1.0f
            );

            // 정점 추가
            meshData.Vertices.push_back(vertex);
        }
    }

    // 옆면의 삼각형 구성
    for (uint32 stack = 0; stack < stackCount; ++stack)
    {
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            uint32 baseIndex = topIndex + 1 + stack * (sliceCount + 1) + slice;

            // 첫 번째 삼각형 (꼭짓점과 연결)
            meshData.Indices.push_back(topIndex);
            meshData.Indices.push_back(baseIndex + 1);
            meshData.Indices.push_back(baseIndex);

            // 두 번째 삼각형 (옆면의 삼각형)
            if (stack < stackCount - 1) // 마지막 스택은 삼각형을 구성하지 않음
            {
                meshData.Indices.push_back(baseIndex);
                meshData.Indices.push_back(baseIndex + 1);
                meshData.Indices.push_back(baseIndex + (sliceCount + 1));

                meshData.Indices.push_back(baseIndex + 1);
                meshData.Indices.push_back(baseIndex + (sliceCount + 1) + 1);
                meshData.Indices.push_back(baseIndex + (sliceCount + 1));
            }
        }
    }
}