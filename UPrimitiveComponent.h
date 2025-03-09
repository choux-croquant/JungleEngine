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
    MeshData meshData;
};