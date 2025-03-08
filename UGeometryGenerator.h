#pragma once
#include "Types.h"
#include "Utils.h"

class UGeometryGenerator
{
public:
    static void CreateSphere(int32 radius, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    static void CreateCylinder(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    static void CreateCylinderTop(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    static void CreateCylinderBottom(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    static void CreateCone(float bottomRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
};