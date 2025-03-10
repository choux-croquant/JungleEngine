#include "ScenePropertyWindow.h"
#include "ImGui/imgui.h"
#include "Matrix.h"
#include "Quaternion.h"


ScenePropertyWindow::ScenePropertyWindow(UCamera& camera)
{
	mainCamera = &camera;
	bIsOrthogonal = false;
	UpdateCamera();
}

void ScenePropertyWindow::SetCameraLocation()
{
	cameraPosition[0] = mainCamera->RelativeLocation.X;
	cameraPosition[1] = mainCamera->RelativeLocation.Y;
	cameraPosition[2] = mainCamera->RelativeLocation.Z;
}

void ScenePropertyWindow::SetCameraRotation()
{
	cameraRotation[0] = RadtoDeg(mainCamera->RelativeRotation.X);
	cameraRotation[1] = RadtoDeg(mainCamera->RelativeRotation.Y);
	cameraRotation[2] = RadtoDeg(mainCamera->RelativeRotation.Z);
}

void ScenePropertyWindow::UpdateCamera()
{
	SetCameraLocation();
	SetCameraRotation();
	FOV = RadtoDeg(mainCamera->FOV);
}

void ScenePropertyWindow::Draw()
{
	//카메라 성질
	ImGui::Checkbox("Orthogonal", &bIsOrthogonal);
	if (mainCamera->bIsOrthogonal != bIsOrthogonal) {
		mainCamera->bIsOrthogonal = bIsOrthogonal;
		if (bIsOrthogonal) {
			mainCamera->RelativeLocation = FVector(-10.0f, 0.0f, 0.0f);
			mainCamera->ResetRotation();
		}
		mainCamera->Update();
		UpdateCamera();
	}
	ImGui::DragFloat("FOV", &FOV, 0.1f, 30.0f, 120.0f, "%.3f");
	float fov = DegtoRad(FOV);
	if (fov != mainCamera->FOV) {
		mainCamera->ChangeFOV(fov);
	}
	ImGui::DragFloat3("Camera Location", cameraPosition, 0.1f, 0.0f, 0.0f, "%.3f");
	FVector newPos(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (newPos != mainCamera->RelativeLocation) {
		mainCamera->SetWorldLocation(newPos);
		mainCamera->Update();
	}
	ImGui::DragFloat3("Camera Rotation", cameraRotation, 0.1f, 0.0f, 0.0f, "%.3f");
	FVector newRot(DegtoRad(cameraRotation[0]), DegtoRad(cameraRotation[1]), DegtoRad(cameraRotation[2]));
	if (newRot!=mainCamera->RelativeRotation) {
		if (newRot.X == 0 && newRot.Y == 0 && newRot.Z == 0) {
			// 모든 회전값이 0이면, 기본값으로 초기화
			mainCamera->ResetRotation();
		}
		else if (newRot.X == 0 || newRot.Y == 0 || newRot.Z == 0) {
			Quaternion rotX = Quaternion::RotateX(newRot.X);
			Quaternion rotY = Quaternion::RotateY(newRot.Y);
			Quaternion rotZ = Quaternion::RotateZ(newRot.Z);
			Quaternion rotation = rotZ * rotY * rotX;
			mainCamera->SetRotation(rotation);
		}
		else {
			Quaternion rotX = Quaternion::RotateX(newRot.X - mainCamera->RelativeRotation.X);
			Quaternion rotY = Quaternion::RotateY(newRot.Y - mainCamera->RelativeRotation.Y);
			Quaternion rotZ = Quaternion::RotateZ(newRot.Z - mainCamera->RelativeRotation.Z);
			Quaternion rotation = rotZ * rotY * rotX;
			mainCamera->RotateByQuaternion(rotation);
		}
		mainCamera->RelativeRotation = newRot;
		mainCamera->Update();
	}
	ImGui::Separator();
}
