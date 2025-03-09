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
	void PickedObjPropertyRender();

	bool rayCollision = false;
	UPrimitiveComponent* closestHitObject = nullptr;
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


	void RayCast();
	void checkCollision();
	bool lineTriangleInter(FVector v0, FVector v1, FVector v2);
	void checkFaceCollision();
	FVector TransformVertexToWorld(const FVector& localVertex, const USceneComponent* component);
};

