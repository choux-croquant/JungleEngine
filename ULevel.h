#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "UObject.h"
#include "UPrimitiveComponent.h"

class ULevel : public UObject {
public:
    ULevel() {}
    ULevel(TArray<UPrimitiveComponent*> components) : PrimitiveComponents(components) {}
    virtual ~ULevel() {
        for (UPrimitiveComponent* Component : PrimitiveComponents) {
            delete Component;
        }
        PrimitiveComponents.clear();
    }

    // primitive ���� �̺�Ʈ �ݹ�
    using PrimitiveSpawnedCallback = std::function<void(UPrimitiveComponent*)>;
    TArray<PrimitiveSpawnedCallback> OnPrimitiveSpawned;

    template<typename T, typename... Args>
    T* SpawnPrimitive(Args&&... args) {
        T* NewPrimitive = new T(std::forward<Args>(args)...);
        PrimitiveComponents.push_back(NewPrimitive);

        // ���� �� ��ϵ� ��� �ݹ鿡 �˸�
        for (auto& callback : OnPrimitiveSpawned) {
            callback(NewPrimitive);
        }
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

    const TArray<UPrimitiveComponent*>& GetPrimitives() const
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

    int GetPrimitiveCount() const {
        return static_cast<int>(PrimitiveComponents.size());
    }

private:
    TArray<UPrimitiveComponent*> PrimitiveComponents;
};