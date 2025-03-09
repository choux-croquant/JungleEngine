#pragma once
#include "USceneComponent.h"
#include "Utils.h"

class UPrimitiveComponent : public USceneComponent
{
public:
    UPrimitiveComponent(FVector Position, FVector Rotation, FVector Scale)
        : USceneComponent(Position, Rotation, Scale)
    {
    }

    TArray<FVertexSimple> Vertices;
    TArray<uint32> Indices;
};