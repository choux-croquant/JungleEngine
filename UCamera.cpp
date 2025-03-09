#include "UCamera.h"
#include <math.h>

UCamera::UCamera(FVector pos, FVector targetpos, FVector up)
    : USceneComponent(pos, FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f)), // �θ� ������ ȣ��
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
    RelativeRotation = GetRotation(); // ����ȭ
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
    float angleY = asinf(R.M[0][2]);              // ��ġ
    float angleZ = atan2f(-R.M[0][1], R.M[0][0]);   // ��
    float angleX = atan2f(-R.M[1][2], R.M[2][2]);   // ��
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
    float rotateSpeed = 0.5f * deltaTime; // ȸ�� �ӵ�

    FVector right = upDirection.Cross(facing).Normalize();
    FVector moveDir = FVector(0.0f, 0.0f, 0.0f);

    // �̵� �Է� ó��
    if (input.IsKeyDown('W')) {
        moveDir -= facing;  // ����
    }
    if (input.IsKeyDown('S')) {
        moveDir += facing;  // ����
    }
    if (input.IsKeyDown('A')) {
        moveDir += right;  // ���� �̵�
    }
    if (input.IsKeyDown('D')) {
        moveDir -= right;  // ���� �̵�
    }
    if (input.IsKeyDown('Q')) {
        moveDir -= upDirection;  // �Ʒ� �̵�
    }
    if (input.IsKeyDown('E')) {
        moveDir += upDirection;  // ���� �̵�
    }

    // ȸ�� �Է� ó�� �׽�Ʈ
    if (input.IsKeyDown(VK_LEFT)) { // ��ȸ�� (Yaw)
        FMatrix yawRotation = FMatrix::RotateZ(rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_RIGHT)) { // ��ȸ�� (Yaw)
        FMatrix yawRotation = FMatrix::RotateZ(-rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_UP)) { // ��ȸ�� (Pitch)
        FMatrix pitchRotation = FMatrix::Rotate(right, rotateSpeed);
        Rotate(pitchRotation);
    }
    if (input.IsKeyDown(VK_DOWN)) { // ��ȸ�� (Pitch)
        FMatrix pitchRotation = FMatrix::Rotate(right, -rotateSpeed);
        Rotate(pitchRotation);
    }

    // ���콺 ��Ŭ�� �巡�׷� ī�޶� ȸ��
    if (input.IsMouseButtonDown(VK_RBUTTON)) { // 1�� ���콺 ��Ŭ�� ��ư
        POINT mouseDelta = input.GetMouseDelta();
        float yaw = -mouseDelta.x * rotateSpeed; // Yaw ȸ�� (�¿�)
        float pitch = mouseDelta.y * rotateSpeed; // Pitch ȸ�� (����)

        // Yaw ȸ�� (�¿�)
        FMatrix yawRotation = FMatrix::RotateZ(yaw);
        Rotate(yawRotation);

        // Pitch ȸ�� (����)
        FMatrix pitchRotation = FMatrix::Rotate(right, pitch);
        Rotate(pitchRotation);
    }

    // �̵� ����
    if (!moveDir.IsZero()) {
        moveDir.Normalize();
        Translate(moveDir * moveSpeed);
    }
}

void UCamera::Update()
{
    viewMatrix = FMatrix::LookAt(GetWorldLocation(), targetPos, upDirection);
}
