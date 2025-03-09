#pragma once
#include "USceneComponent.h"
#include "Utils.h"
#include "Matrix.h"

enum class EPrimitiveType {
    Cube,
    Sphere,
    Triangle,
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
    MeshData meshData;

    virtual void Render(FMatrix view, FMatrix projection) = 0;

    const FString GetTypeName() const {
        switch (PrimitiveType) {
        case EPrimitiveType::Cube: return "Cube";
        case EPrimitiveType::Sphere: return "Sphere";
        case EPrimitiveType::Triangle: return "Triangle";
        case EPrimitiveType::Cylinder: return "Cylinder";
        case EPrimitiveType::Cone: return "Cone";
        default: return "Unknown";
        }
    }

    static const EPrimitiveType GetType(FString type) {
        if (type == "Cube") {
            return EPrimitiveType::Cube;
        }
        if (type == "Sphere") {
            return EPrimitiveType::Sphere;
        }
        if (type == "Triangle") {
            return EPrimitiveType::Triangle;
        }
    }

    uint32 GetUUID() {
        return UUID;
    }
    FVector GetLocation() {
        return GetWorldLocation();
    }

    FVector GetRotation() {
        return GetWorldRotation();
    }

    FVector GetScale() {
        return GetWorldScale3D();
    }
private:
    EPrimitiveType PrimitiveType;
};