#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "UObject.h"
#include "UPrimitiveComponent.h"

// TODO : ���� Actor���� ���� ���·� ��ȯ
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