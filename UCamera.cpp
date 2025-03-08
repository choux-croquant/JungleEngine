#include "UCamera.h"
#include <math.h>

UCamera::UCamera(FVector pos, FVector targetpos, FVector up)
    : USceneComponent(pos, FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f)), // 부모 생성자 호출
    originalPos(pos), targetPos(targetpos)
{
    facing = (pos - targetPos).Normalize();

    if (pos.Y == targetPos.Y) {
        originalUp = up;
        upDirection = originalUp;
    }
    else {
        FVector axis = FVector(0, 0, 1).Cross(facing).Normalize();
        FMatrix rotation = FMatrix::Rotate(axis, acosf(FVector(0, 0, 1).Dot(facing)));
        originalUp = rotation * up;
        upDirection = originalUp;
    }

    viewMatrix = FMatrix::LookAt(pos, targetPos, upDirection);
    FOV = PI / 4.0f;
    projectionMatrix = FMatrix::Perspective(FOV, 1.0f, 1.0f, 100.0f);
}

void UCamera::Rotate(FMatrix rotationMatrix)
{
    facing = rotationMatrix * facing;
    targetPos = facing + GetWorldLocation();
    upDirection = rotationMatrix * upDirection;
    upDirection.Normalize();
    RelativeRotation = GetWorldRotation(); // 동기화
}

void UCamera::Translate(FVector offset)
{
    RelativeLocation += offset;
    targetPos = facing + GetWorldLocation();
}

void UCamera::ChangeFOV(float fov)
{
    FOV = fov;
    projectionMatrix = FMatrix::Perspective(FOV, 1.0f, 1.0f, 100.0f);
}

void UCamera::SetWorldLocation(FVector pos)
{
    RelativeLocation = pos;
    targetPos = facing + GetWorldLocation();
}

void UCamera::MoveCamera(const InputManager& input, float deltaTime) {
    float moveSpeed = 5.0f * deltaTime;
    float rotateSpeed = 1.0f * deltaTime; // 회전 속도

    FVector right = upDirection.Cross(facing).Normalize();
    FVector moveDir = FVector(0.0f, 0.0f, 0.0f);

    // 이동 입력 처리
    if (input.IsKeyDown('W')) {
        moveDir -= facing;  // 전진
    }
    if (input.IsKeyDown('S')) {
        moveDir += facing;  // 후진
    }
    if (input.IsKeyDown('A')) {
        moveDir -= right;  // 좌측 이동
    }
    if (input.IsKeyDown('D')) {
        moveDir += right;  // 우측 이동
    }
    if (input.IsKeyDown('Q')) {
        moveDir -= upDirection;  // 아래 이동
    }
    if (input.IsKeyDown('E')) {
        moveDir += upDirection;  // 위로 이동
    }

    // 회전 입력 처리 테스트
    if (input.IsKeyDown(VK_LEFT)) { // 좌회전 (Yaw)
        FMatrix yawRotation = FMatrix::RotateY(rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_RIGHT)) { // 우회전 (Yaw)
        FMatrix yawRotation = FMatrix::RotateY(-rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_UP)) { // 상회전 (Pitch)
        FMatrix pitchRotation = FMatrix::Rotate(right, rotateSpeed);
        Rotate(pitchRotation);
    }
    if (input.IsKeyDown(VK_DOWN)) { // 하회전 (Pitch)
        FMatrix pitchRotation = FMatrix::Rotate(right, -rotateSpeed);
        Rotate(pitchRotation);
    }

    // 이동 적용
    if (!moveDir.IsZero()) {
        moveDir.Normalize();
        Translate(moveDir * moveSpeed);
    }
}
void UCamera::Update()
{
    viewMatrix = FMatrix::LookAt(GetWorldLocation(), targetPos, upDirection);
}
