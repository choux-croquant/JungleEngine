#pragma once
#include "Vector.h"
#include "UCamera.h"

class ScenePropertyWindow {
public:
	float FOV = 45;
	float cameraPosition[3];
	float cameraRotation[3];
	bool bIsOrthogonal;
	UCamera* mainCamera;

public:
	ScenePropertyWindow(UCamera& camera);
	void SetCameraLocation();
	void SetCameraRotation();
	void UpdateCamera();
	void Draw();
};