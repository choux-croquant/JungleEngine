#pragma once
#include "Vector.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Types.h"

struct FVertexSimple
{
	FVector Position;
	FVector4 Color;
};


struct FConstants
{
	FMatrix MVP;
};

struct MeshData 
{
	TArray<FVertexSimple> Vertices;
	TArray<uint32> Indices;
};