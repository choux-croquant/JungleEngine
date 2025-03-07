#pragma once
#include "Vector.h"
#include "Vector4.h"
#include "Matrix.h"

struct FVertexSimple
{
	FVector Position;
	FVector4 Color;
};


struct FConstants
{
	FMatrix MVP;
};