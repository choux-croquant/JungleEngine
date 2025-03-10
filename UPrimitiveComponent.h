#pragma once
#include "USceneComponent.h"
#include "Utils.h"
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
        : USceneComponent(Position, Rotation, Scale), PrimitiveType(type)
    {
        bIsClicked = false;
    }
    MeshData meshData;

    virtual void Render(FMatrix view, FMatrix projection) = 0;

    const FString GetTypeName() const {
        switch (PrimitiveType) {
        case EPrimitiveType::Cube: return "Cube";
        case EPrimitiveType::Sphere: return "Sphere";
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
        if (type == "Cylinder") {
            return EPrimitiveType::Cylinder;
        }
        if (type == "Cone") {
            return EPrimitiveType::Cone;
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
public:
    bool bIsClicked;
};