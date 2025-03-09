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
    RelativeRotation = GetRotation(); // 동기화
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

void UCamera::MoveCamera(float deltaTime) {
    InputManager& input = InputManager::GetInstance();
    float moveSpeed = 5.0f * deltaTime;
    float rotateSpeed = 0.5f * deltaTime; // 회전 속도

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
        moveDir += right;  // 좌측 이동
    }
    if (input.IsKeyDown('D')) {
        moveDir -= right;  // 우측 이동
    }
    if (input.IsKeyDown('Q')) {
        moveDir -= upDirection;  // 아래 이동
    }
    if (input.IsKeyDown('E')) {
        moveDir += upDirection;  // 위로 이동
    }

    // 회전 입력 처리 테스트
    if (input.IsKeyDown(VK_LEFT)) { // 좌회전 (Yaw)
        FMatrix yawRotation = FMatrix::RotateZ(rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_RIGHT)) { // 우회전 (Yaw)
        FMatrix yawRotation = FMatrix::RotateZ(-rotateSpeed);
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

    // 마우스 우클릭 드래그로 카메라 회전
    if (input.IsMouseButtonDown(VK_RBUTTON)) { // 1은 마우스 우클릭 버튼
        POINT mouseDelta = input.GetMouseDelta();
        float yaw = -mouseDelta.x * rotateSpeed; // Yaw 회전 (좌우)
        float pitch = mouseDelta.y * rotateSpeed; // Pitch 회전 (상하)

        // Yaw 회전 (좌우)
        FMatrix yawRotation = FMatrix::RotateZ(yaw);
        Rotate(yawRotation);

        // Pitch 회전 (상하)
        FMatrix pitchRotation = FMatrix::Rotate(right, pitch);
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
