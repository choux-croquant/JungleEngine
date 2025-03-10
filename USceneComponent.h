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

    // �θ�� �ڽ� ����
    USceneComponent* Parent;
    TArray<USceneComponent*> Children;

    // ��� ��ǥ
    FVector RelativeLocation;
    FVector RelativeRotation;
    FVector RelativeScale3D;

    // �θ� ����
    void AttachTo(USceneComponent* NewParent)
    {
        if (Parent)
            DetachFromParent();  // ���� �θ𿡼� �и�

        Parent = NewParent;
        if (NewParent)
            NewParent->Children.push_back(this);
    }

    // �θ���� ���� ����
    void DetachFromParent()
    {
        if (Parent)
        {
            auto& siblings = Parent->Children;
            siblings.erase(remove(siblings.begin(), siblings.end(), this), siblings.end());
            Parent = nullptr;
        }
    }

    // ���� ��ġ ��� (�θ��� Ʈ�������� ����)
    FVector GetWorldLocation() const
    {
        if (Parent)
            return Parent->GetWorldLocation() + RelativeLocation; // �θ� ��ġ + ��� ��ġ
        return RelativeLocation;
    }

    // ���� ȸ�� ���
    FVector GetWorldRotation() const
    {
        if (Parent)
            return Parent->GetWorldRotation() + RelativeRotation; // �θ� ȸ�� + ��� ȸ��
        return RelativeRotation;
    }

    // ���� ������ ���
    FVector GetWorldScale3D() const
    {
        if (Parent)
            return Parent->GetWorldScale3D() * RelativeScale3D; // �θ� ������ * ��� ������
        return RelativeScale3D;
    }

    FMatrix GetWorldTransform() const
    {
        // ���� ���� ��ȯ ���
        FMatrix localTransform =
            FMatrix::Translate(RelativeLocation) *
            (FMatrix::RotateZ(RelativeRotation.Z) *
                FMatrix::RotateY(RelativeRotation.Y) *
                FMatrix::RotateX(RelativeRotation.X)) *
            FMatrix::Scale(RelativeScale3D);

        if (Parent)
        {
            // �θ��� ���� ��ȯ ���
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
