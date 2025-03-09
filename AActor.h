#pragma once
#include "UObject.h"

class UActorComponent;

class AActor : public UObject
{
public:
    AActor();
    virtual ~AActor();

    // ���Ͱ� ������ ������Ʈ ���
    TArray<UActorComponent*> Components;

    // ������Ʈ �߰�
    void AddComponent(UActorComponent* Component);

    // �� ������ ȣ��Ǵ� ������Ʈ �Լ�
    virtual void Tick(float DeltaTime);
};