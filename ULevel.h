#pragma once
#include <vector>
#include <memory>
#include "UObject.h"
#include "UPrimitiveComponent.h"

class ULevel : public UObject {
public:
    ULevel() {}
    virtual ~ULevel() {
        for (UPrimitiveComponent* Component : PrimitiveComponents) {
            delete Component;
        }
        PrimitiveComponents.clear();
    }

    template<typename T, typename... Args>
    T* SpawnPrimitive(Args&&... args) {
        T* NewPrimitive = new T(std::forward<Args>(args)...);
        PrimitiveComponents.push_back(NewPrimitive);
        return NewPrimitive;
    }

    UPrimitiveComponent* SpawnPrimitiveByType(EPrimitiveType type,
        const FVector& pos,
        const FVector& rot,
        const FVector& scale)
    {
        switch (type) {
        case EPrimitiveType::Cube:
            return SpawnPrimitive<UCubeComp>(pos, rot, scale);
        case EPrimitiveType::Sphere:
            return SpawnPrimitive<USphereComp>(pos, rot, scale);
        case EPrimitiveType::Cylinder:
            return SpawnPrimitive<UCylinderComp>(pos, rot, scale);
        case EPrimitiveType::Cone:
            return SpawnPrimitive<UConeComp>(pos, rot, scale);
        default:
            return nullptr;
        }
    }
    TArray<UPrimitiveComponent*> GetPrimitives()
    {
        return PrimitiveComponents;
    }

    void Tick(float DeltaTime) {
        for (UPrimitiveComponent* Component : PrimitiveComponents) {
            if (Component) {
                Component->TickComponent(DeltaTime);
            }
        }
    }

private:
    TArray<UPrimitiveComponent*> PrimitiveComponents;
};
