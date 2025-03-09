#pragma once
#include "UObject.h"

class UActorComponent;

class AActor : public UObject
{
public:
    AActor();
    virtual ~AActor();

    // 액터가 소유한 컴포넌트 목록
    TArray<UActorComponent*> Components;

    // 컴포넌트 추가
    void AddComponent(UActorComponent* Component);

    // 매 프레임 호출되는 업데이트 함수
    virtual void Tick(float DeltaTime);
};