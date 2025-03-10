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
	FPhysScene(HWND hwnd,UCamera* camera);
	void SetPrimitive(UPrimitiveComponent* uPrimitiveComp);
	void SetGizmo(UPrimitiveComponent* cylinder, 
		UPrimitiveComponent* cone,
		GizmoAxis gizmoAxis);
	void SetGizmoGroup(USceneComponent* gizmoGroup);
	void Update();
	void LogRender();
	void PickedObjPropertyRender();


	bool rayCollision = false;
	UPrimitiveComponent* closestHitObject = nullptr;
private:
	TArray<UPrimitiveComponent*> primitives;
	InputManager& input = InputManager::GetInstance();
	HWND hwnd;
	UCamera* camera;

	USceneComponent* m_gizmoGroup;
	TArray<Gizmo> gizmos;
	Gizmo CurrentGizmo;
	bool isGizmoClicked = false;

	POINT mousePos;
	int width, height;

	FVector4 ndc;
	FVector4 rayView;
	
	//�ѹ� Ŭ���Ҷ� Ray
	FVector rayWorld;
	FVector rayDir;

	//�巡�� �� Ray
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

