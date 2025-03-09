#include "ScenePropertyWindow.h"
#include "ImGui/imgui.h"
#include "Matrix.h"


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
	ImGui::InputFloat("FOV", &FOV, 0.0f, 0.0f, "%.3f");
	float fov = DegtoRad(FOV);
	if (fov != mainCamera->FOV) {
		mainCamera->ChangeFOV(fov);
	}
	ImGui::InputFloat3("Camera Location", cameraPosition, "%.3f");
	FVector newPos(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (newPos != mainCamera->RelativeLocation) {
		mainCamera->SetWorldLocation(newPos);
		mainCamera->Update();
	}
	ImGui::InputFloat3("Camera Rotation", cameraRotation, "%.3f");
	FVector newRot(DegtoRad(cameraRotation[0]), DegtoRad(cameraRotation[1]), DegtoRad(cameraRotation[2]));
	if (newRot != mainCamera->RelativeRotation) {
		FMatrix rotationX = FMatrix::RotateX(newRot.X - mainCamera->RelativeRotation.X);
		FMatrix rotationY = FMatrix::RotateY(newRot.Y - mainCamera->RelativeRotation.Y);
		FMatrix rotationZ = FMatrix::RotateZ(newRot.Z - mainCamera->RelativeRotation.Z);
		FMatrix finalRotation = rotationZ * rotationY * rotationX;
		mainCamera->Rotate(finalRotation);
		mainCamera->RelativeRotation = mainCamera->GetRotation();
		mainCamera->Update();
	}
	ImGui::Separator();
}
