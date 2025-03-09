#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "UObject.h"
#include "UPrimitiveComponent.h"

// TODO : 추후 Actor들을 가진 형태로 변환
class ULevel : public UObject {
public:
    ULevel() {}
    virtual ~ULevel() {}

    void AddPrimitiveComponent(UPrimitiveComponent* Component) {
        if (Component) {
            PrimitiveComponents.push_back(Component);
        }
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