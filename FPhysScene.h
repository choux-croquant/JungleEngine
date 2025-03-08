#pragma once
#include "InputManager.h"
#include "UCubeComp.h"
#include "Types.h"
#include "Matrix.h"
class FPhysScene
{
public:
	FPhysScene(HWND hwnd);
	void setSampleCube(const UCubeComp& uCubeComp);
	void Update();
	void LogRender();
private:
	TArray<UPrimitiveComponent> cubes;
	InputManager& input = InputManager::GetInstance();
	HWND hwnd;

	POINT mousePos;
	int width, height;

	FVector ndc;
};

