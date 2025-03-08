#pragma once
#include "USceneComponent.h"
#include "Matrix.h"
#include "InputManager.h"

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

    UCamera(FVector pos, FVector target, FVector up);

    void Rotate(FMatrix rotationMatrix);
    void Translate(FVector offset);
    void ChangeFOV(float fov);
    void SetWorldLocation(FVector pos);
    void MoveCamera(const InputManager& input, float deltaTime);
    void Update();
};
