#include "UCamera.h"
#include <math.h>

UCamera::UCamera(FVector pos, FVector targetpos, FVector up)
    : USceneComponent(pos, FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f)), // �θ� ������ ȣ��
    originalPos(pos), targetPos(targetpos)
{
    facing = (targetPos - pos).Normalize();

    if (pos.Z == targetPos.Z) {
        originalUp = up;
        upDirection = originalUp;
    }
    else {
        FVector axis = FVector(1, 0, 0).Cross(facing).Normalize();
        FMatrix rotation = FMatrix::Rotate(axis, acosf(FVector(1, 0, 0).Dot(facing)));
        originalUp = rotation * up;
        upDirection = originalUp;
    }
    RelativeRotation = GetRotation();
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

void UCamera::RotateByQuaternion(Quaternion rotation)
{
    facing = rotation.RotateVector(facing);
    targetPos = facing + GetWorldLocation();
    upDirection = rotation.RotateVector(upDirection);
    upDirection.Normalize();
    RelativeRotation = GetRotation(); // ����ȭ
}

void UCamera::SetRotation(Quaternion rotation)
{
    facing = rotation.RotateVector(FVector(1, 0, 0));
    targetPos = facing + GetWorldLocation();
    upDirection = rotation.RotateVector(FVector(0, 0, 1));
    RelativeRotation = GetRotation();
}

void UCamera::ResetRotation()
{
    facing = FVector(1, 0, 0);
    targetPos = facing + GetWorldLocation();
    upDirection = FVector(0, 0, 1);
    RelativeRotation = ZeroVector;
}

FVector UCamera::GetRotation()
{
    FVector zAxis = upDirection.Normalize();
    FVector xAxis = facing.Normalize();
    FVector yAxis = zAxis.Cross(xAxis);

    FMatrix R = FMatrix::Identity;
    R.M[0][0] = xAxis.X; R.M[0][1] = yAxis.X; R.M[0][2] = zAxis.X;
    R.M[1][0] = xAxis.Y; R.M[1][1] = yAxis.Y; R.M[1][2] = zAxis.Y;
    R.M[2][0] = xAxis.Z; R.M[2][1] = yAxis.Z; R.M[2][2] = zAxis.Z;

    R.Transpose();
    float angleY = asinf(R.M[0][2]);             
    float angleZ = atan2f(-R.M[0][1], R.M[0][0]); 
    float angleX = atan2f(-R.M[1][2], R.M[2][2]); 

    return FVector(angleX, angleY, angleZ);
}

FVector UCamera::GetRotation(Quaternion rotation)
{
    float yaw;
    float roll;
    float pitch;
    float W = rotation.W;
    float X = rotation.X;
    float Y = rotation.Y;
    float Z = rotation.Z;
    yaw = atan2(2.0f * (W * Y + X * Z), 1.0f - 2.0f * (Y * Y + Z * Z));

    // Pitch (X�� ȸ��) - �״�� ����
    float sinp = 2.0f * (W * X - Y * Z);
    if (fabs(sinp) >= 1)
        pitch = copysign(PI/2.0f, sinp);
    else
        pitch = asin(sinp);

    // Roll (Z�� ȸ��) - ��ȣ ����
    roll = atan2(2.0f * (W * Z + X * Y), 1.0f - 2.0f * (X * X + Z * Z));

    return FVector(pitch, yaw, roll);
}

void UCamera::Translate(FVector offset)
{
    RelativeLocation += offset;
    targetPos = facing + GetWorldLocation();
}

void UCamera::ChangeFOV(float fov)
{
    FOV = fov;
    Update();
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
        moveDir += facing;  // ����
    }
    if (input.IsKeyDown('S')) {
        moveDir -= facing;  // ����
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
        Quaternion yawRotation = Quaternion::RotateZ(-rotateSpeed);
        RotateByQuaternion(yawRotation);
    }
    if (input.IsKeyDown(VK_RIGHT)) { // ��ȸ�� (Yaw)
        Quaternion yawRotation = Quaternion::RotateZ(rotateSpeed);
        RotateByQuaternion(yawRotation);
    }
    if (input.IsKeyDown(VK_UP)) { // ��ȸ�� (Pitch)
        Quaternion pitchRotation = Quaternion::Rotate(right, -rotateSpeed);
        RotateByQuaternion(pitchRotation);
    }
    if (input.IsKeyDown(VK_DOWN)) { // ��ȸ�� (Pitch)
        Quaternion pitchRotation = Quaternion::Rotate(right, rotateSpeed);
        RotateByQuaternion(pitchRotation);
    }

    // ���콺 ��Ŭ�� �巡�׷� ī�޶� ȸ��
    if (input.IsMouseButtonDown(VK_RBUTTON)) { // 1�� ���콺 ��Ŭ�� ��ư
        POINT mouseDelta = input.GetMouseDelta();
        float yaw = -mouseDelta.x * rotateSpeed; // Yaw ȸ�� (�¿�)
        float pitch = -mouseDelta.y * rotateSpeed; // Pitch ȸ�� (����)

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
    if (bIsOrthogonal) {
        projectionMatrix = FMatrix::Orthogonal(-10, 10, -10, 10, -10, 10);
    }
    else {
        projectionMatrix = FMatrix::Perspective(FOV, 1.0f, 1.0f, 100.0f);
    }
    viewMatrix = FMatrix::LookAt(GetWorldLocation(), targetPos, upDirection);
}
