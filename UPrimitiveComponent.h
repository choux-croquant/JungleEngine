#pragma once
#include "USceneComponent.h"
#include "Matrix.h"

enum class EPrimitiveType {
    Cube,
    Sphere,
    Cylinder,
    Cone,
    Gizmo,
    Axis
};

class UPrimitiveComponent : public USceneComponent
{
public:
    UPrimitiveComponent(EPrimitiveType type, FVector Position, FVector Rotation, FVector Scale)
        : USceneComponent(Position, Rotation, Scale)
    {
    }

    virtual void Render(FMatrix view, FMatrix projection) = 0;

    const char* GetTypeName() const {
        switch (PrimitiveType) {
        case EPrimitiveType::Cube: return "Cube";
        case EPrimitiveType::Sphere: return "Sphere";
        case EPrimitiveType::Cylinder: return "Cylinder";
        case EPrimitiveType::Cone: return "Cone";
        default: return "Unknown";
        }
    }
private:
    EPrimitiveType PrimitiveType;

    
};