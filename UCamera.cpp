#include "UCamera.h"
#include <math.h>



UCamera::UCamera(FVector pos, FVector targetpos, FVector up) : originalPos(pos), targetPos(targetpos)
{
	position = originalPos;
	FVector forward = (targetPos - pos);
	facing = forward;
	forward.Normalize();
	if (pos.Y == targetPos.Y) {
		originalUp = up;
		upDirection = originalUp;
	}
	else {
		FVector axis = FVector(0, 0, 1).Cross(forward);
		axis.Normalize();
		FMatrix rotation = FMatrix::Rotate(axis, acosf(FVector(0, 0, 1).Dot(forward)));
		originalUp = rotation * up;
		upDirection = originalUp;
	}
	viewMatrix = FMatrix::LookAt(pos, targetPos, upDirection);
	FOV = PI / 4.0f;
	projectionMatrix = FMatrix::Perspective(FOV, 1.0f, 1.0f, 100.0f);
	rotation = ZeroVector;
}

void UCamera::Rotate(FMatrix rotationMatrix)
{
	facing = rotationMatrix * facing;
	targetPos = facing + position;
	upDirection = rotationMatrix * upDirection;
	upDirection.Normalize();
	viewMatrix = FMatrix::LookAt(position, targetPos, upDirection);
	rotation = GetRotation();
}

void UCamera::Translate(FVector offset)
{
	position += offset;
	targetPos = facing + position;
	viewMatrix = FMatrix::LookAt(position, targetPos, upDirection);
}

void UCamera::ChangeFOV(float fov)
{
	FOV = fov;
	projectionMatrix = FMatrix::Perspective(FOV, 1.0f, 1.0f, 100.0f);
}

void UCamera::SetPosition(FVector pos)
{
	originalPos = pos;
	position = originalPos;
	targetPos = facing + position;
	viewMatrix = FMatrix::LookAt(position, targetPos, upDirection);
}

FVector UCamera::GetRotation()
{
	FVector zAxis = facing.Normalize();
	FVector yAxis = upDirection;
	FVector xAxis = yAxis.Cross(zAxis).Normalize();
	FMatrix R = FMatrix::Identity;
	R.M[0][0] = xAxis.X; R.M[0][1] = yAxis.X; R.M[0][2] = zAxis.X;
	R.M[1][0] = xAxis.Y; R.M[1][1] = yAxis.Y; R.M[1][2] = zAxis.Y;
	R.M[2][0] = xAxis.Z; R.M[2][1] = yAxis.Z; R.M[2][2] = zAxis.Z;
	R.Transpose();
	float angleY = asinf(R.M[0][2]);              // 피치
	float angleZ = atan2f(-R.M[0][1], R.M[0][0]);   // 요
	float angleX = atan2f(-R.M[1][2], R.M[2][2]);   // 롤
	return FVector(angleX, angleY, angleZ);
}


