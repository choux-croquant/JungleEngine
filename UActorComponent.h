#pragma once
#include "UObject.h"
#include "AActor.h"

class UActorComponent : public UObject
{
public:
	UActorComponent() : Owner(nullptr), bIsActive(true) {};

	// ���� �� ���� ���� (���ʿ��� ��ü ���� ���� ����)
	UActorComponent(const UActorComponent&) = delete;
	UActorComponent& operator=(const UActorComponent&) = delete;

	// ������Ʈ�� �����ϴ� ���� ������
	AActor* Owner;

	// ������Ʈ Ȱ��ȭ ����
	bool bIsActive;

	// ������Ʈ �ʱ�ȭ : ���ҽ� �Ҵ��̳� �̺�Ʈ ���ε� �� �ʱ� ����
	virtual void InitializeComponent() {}

	// ���� ���� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() {}

	// �� ������ ������Ʈ (�ʿ��� ��� DeltaTime��ŭ ������Ʈ)
	virtual void TickComponent(float DeltaTime) {}

	virtual void Activate() { bIsActive = true; }
	virtual void DeActivate() { bIsActive = false; }
};