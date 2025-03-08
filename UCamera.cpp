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
    RelativeRotation = GetWorldRotation(); // ����ȭ
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
    float rotateSpeed = 1.0f * deltaTime; // ȸ�� �ӵ�

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
        moveDir -= right;  // ���� �̵�
    }
    if (input.IsKeyDown('D')) {
        moveDir += right;  // ���� �̵�
    }
    if (input.IsKeyDown('Q')) {
        moveDir -= upDirection;  // �Ʒ� �̵�
    }
    if (input.IsKeyDown('E')) {
        moveDir += upDirection;  // ���� �̵�
    }

    // ȸ�� �Է� ó�� �׽�Ʈ
    if (input.IsKeyDown(VK_LEFT)) { // ��ȸ�� (Yaw)
        FMatrix yawRotation = FMatrix::RotateY(rotateSpeed);
        Rotate(yawRotation);
    }
    if (input.IsKeyDown(VK_RIGHT)) { // ��ȸ�� (Yaw)
        FMatrix yawRotation = FMatrix::RotateY(-rotateSpeed);
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
