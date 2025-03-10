#pragma once
#include "USceneComponent.h"
#include "Matrix.h"
#include "InputManager.h"
#include "Quaternion.h"

class UCamera : public USceneComponent
{
public:
    float FOV;
    FVector originalPos;
    FVector originalUp;
    FVector facing;
    FVector upDirection;
    FVector targetPos;
    FMatrix viewMatrix;
    FMatrix projectionMatrix;
    bool bIsOrthogonal;

    UCamera(FVector pos, FVector target, FVector up);

    void Rotate(FMatrix rotationMatrix);
    void RotateByQuaternion(Quaternion rotation);
    void SetRotation(Quaternion rotation);
    void ResetRotation();
    FVector GetRotation();
    FVector GetRotation(Quaternion rotation);
    void Translate(FVector offset);
    void ChangeFOV(float fov);
    void SetWorldLocation(FVector pos);
    void MoveCamera(float deltaTime);
    void Update();
};
