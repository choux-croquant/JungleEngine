#pragma once
#include "USceneComponent.h"

class UPrimitiveComponent : public USceneComponent
{
public:
    UPrimitiveComponent(FVector Position, FVector Rotation, FVector Scale)
        : USceneComponent(Position, Rotation, Scale)
    {
    }
};