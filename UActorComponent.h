#pragma once
#include "UObject.h"
#include "AActor.h"

class UActorComponent : public UObject
{
public:
	UActorComponent() : Owner(nullptr), bIsActive(true) {};

	// 복사 및 대입 방지 (불필요한 객체 복사 막기 위함)
	UActorComponent(const UActorComponent&) = delete;
	UActorComponent& operator=(const UActorComponent&) = delete;

	// 컴포넌트를 소유하는 액터 포인터
	AActor* Owner;

	// 컴포넌트 활성화 상태
	bool bIsActive;

	// 컴포넌트 초기화 : 리소스 할당이나 이벤트 바인딩 등 초기 설정
	virtual void InitializeComponent() {}

	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() {}

	// 매 프레임 업데이트 (필요한 경우 DeltaTime만큼 업데이트)
	virtual void TickComponent(float DeltaTime) {}

	virtual void Activate() { bIsActive = true; }
	virtual void DeActivate() { bIsActive = false; }
};