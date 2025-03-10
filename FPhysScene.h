#pragma once
#include "InputManager.h"
#include "UCubeComp.h"
#include "Types.h"
#include "Matrix.h"
#include "UCamera.h"

enum class GizmoAxis
{
	X,Y,Z
};

struct Gizmo
{
	UPrimitiveComponent* gizmoCylinder;
	UPrimitiveComponent* gizmoCone;
	GizmoAxis gizmoAxis;
};

class FPhysScene
{
public:
	FPhysScene(HWND hwnd,const UCamera* camera);
	void SetPrimitive(UPrimitiveComponent* uPrimitiveComp);
	void SetGizmo(UPrimitiveComponent* cylinder, 
		UPrimitiveComponent* cone,
		GizmoAxis gizmoAxis);
	void Update();
	void LogRender();
	void PickedObjPropertyRender();


	bool rayCollision = false;
	UPrimitiveComponent* closestHitObject = nullptr;
private:
	TArray<UPrimitiveComponent*> primitives;
	InputManager& input = InputManager::GetInstance();
	HWND hwnd;
	const UCamera* camera;

	TArray<Gizmo> gizmos;
	Gizmo CurrentGizmo;
	bool isGizmoClicked = false;
	USceneComponent* gizmoGroup;

	POINT mousePos;
	int width, height;

	FVector4 ndc;
	FVector4 rayView;
	
	//한번 클릭할때 Ray
	FVector rayWorld;
	FVector rayDir;

	//드래그 중 Ray
	FVector prevRayWorld;
	FVector currentRayWorld;
	FVector deltaRayWorld;


	FVector RayCast();
	void checkCollision();
	bool lineTriangleInter(FVector v0, FVector v1, FVector v2, FVector& outIntersection);
	bool lineMeshIntersection(const UPrimitiveComponent* mesh, FVector& outIntersection);
	void checkFaceCollision();
	FVector TransformVertexToWorld(const FVector& localVertex, const USceneComponent* component);


	void checkGizmo();

	bool prevMouseButtonState = false;
};

