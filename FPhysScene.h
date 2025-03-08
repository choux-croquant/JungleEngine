#pragma once
#include "InputManager.h"
#include "UCubeComp.h"
#include "Types.h"
#include "Matrix.h"
#include "UCamera.h"
class FPhysScene
{
public:
	FPhysScene(HWND hwnd,const UCamera* camera);
	void setSampleCube(UCubeComp* uCubeComp);
	void Update();
	void LogRender();
private:
	TArray<UPrimitiveComponent*> cubes;
	InputManager& input = InputManager::GetInstance();
	HWND hwnd;
	const UCamera* camera;

	POINT mousePos;
	int width, height;

	FVector4 ndc;
	FVector4 rayView;
	FVector rayWorld;

	FVector rayDir;

	bool rayCollision = false;

	void RayCast();
	void checkCollision();
};

