#pragma once
#include "Vector.h"
#include "Matrix.h"

class UCamera {
public: 
	FVector originalPos;
	FVector position;
	FVector targetPos;
	FVector originalUp;
	FVector upDirection;
	FVector facing;
	FVector rotation;
	float FOV;
	FMatrix viewMatrix;
	FMatrix projectionMatrix;

public:
	UCamera(FVector pos = ZeroVector, FVector targetpos = ZeroVector, FVector up = ZeroVector);
	void Rotate(FMatrix rotationMatrix);
	void Translate(FVector offset);
	void ChangeFOV(float fov);
	void SetPosition(FVector pos);
	FVector GetRotation();
};