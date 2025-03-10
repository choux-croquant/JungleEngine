#pragma once
#include "UActorComponent.h"
#include "Vector.h"
#include "Types.h"
#include "Matrix.h"

class USceneComponent : public UActorComponent
{
public:
    USceneComponent(FVector Position = FVector(), FVector Rotation = FVector(), FVector Scale = FVector(1, 1, 1))
        : RelativeLocation(Position), RelativeRotation(Rotation), RelativeScale3D(Scale), Parent(nullptr) {
    }

    // 부모와 자식 관계
    USceneComponent* Parent;
    TArray<USceneComponent*> Children;

    // 상대 좌표
    FVector RelativeLocation;
    FVector RelativeRotation;
    FVector RelativeScale3D;

    // 부모 설정
    void AttachTo(USceneComponent* NewParent)
    {
        if (Parent)
            DetachFromParent();  // 기존 부모에서 분리

        Parent = NewParent;
        if (NewParent)
            NewParent->Children.push_back(this);
    }

    // 부모와의 관계 해제
    void DetachFromParent()
    {
        if (Parent)
        {
            auto& siblings = Parent->Children;
            siblings.erase(remove(siblings.begin(), siblings.end(), this), siblings.end());
            Parent = nullptr;
        }
    }

    // 월드 위치 계산 (부모의 트랜스폼을 적용)
    FVector GetWorldLocation() const
    {
        if (Parent)
            return Parent->GetWorldLocation() + RelativeLocation; // 부모 위치 + 상대 위치
        return RelativeLocation;
    }

    // 월드 회전 계산
    FVector GetWorldRotation() const
    {
        if (Parent)
            return Parent->GetWorldRotation() + RelativeRotation; // 부모 회전 + 상대 회전
        return RelativeRotation;
    }

    // 월드 스케일 계산
    FVector GetWorldScale3D() const
    {
        if (Parent)
            return Parent->GetWorldScale3D() * RelativeScale3D; // 부모 스케일 * 상대 스케일
        return RelativeScale3D;
    }

    FMatrix GetWorldTransform() const
    {
        // 현재 로컬 변환 행렬
        FMatrix localTransform =
            FMatrix::Translate(RelativeLocation) *
            (FMatrix::RotateZ(RelativeRotation.Z) *
                FMatrix::RotateY(RelativeRotation.Y) *
                FMatrix::RotateX(RelativeRotation.X)) *
            FMatrix::Scale(RelativeScale3D);

        if (Parent)
        {
            // 부모의 월드 변환 행렬
            FMatrix parentTransform = Parent->GetWorldTransform();
            return parentTransform * localTransform;
        }
        else
        {
            return localTransform;
        }
        return localTransform;
    }
};
