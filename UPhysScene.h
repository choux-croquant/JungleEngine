#pragma once
#include "UInputManager.h"
#include "UCubeComp.h"
#include "Types.h"
#include "Matrix.h"
#include "UCamera.h"

enum class GizmoAxis
{
	X,Y,Z
};

enum class GizmoMode
{
	TRANSLATE,
	ROTATE,
	SCALE,
};

struct Gizmo
{
	UPrimitiveComponent* gizmoCylinder;
	UPrimitiveComponent* gizmoCone;
	GizmoAxis gizmoAxis;
};

class UPhysScene
{
public:
	UPhysScene(HWND hwnd,UCamera* camera);
	void SetPrimitive(UPrimitiveComponent* uPrimitiveComp);
	void SetGizmo(UPrimitiveComponent* cylinder, 
		UPrimitiveComponent* cone,
		GizmoAxis gizmoAxis);
	void SetGizmoGroup(USceneComponent* gizmoGroup);
	void Update();
	void LogRender();
	void PickedObjPropertyRender();
	
	TArray<UPrimitiveComponent*> GetPrimitives();

	bool rayCollision = false;
	UPrimitiveComponent* closestHitObject = nullptr;
	GizmoMode currentGizmoMode = GizmoMode::TRANSLATE;
private:
	TArray<UPrimitiveComponent*> primitives;
	UInputManager& input = UInputManager::GetInstance();
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
	
	//한번 클릭할때 Ray
	FVector rayWorld;
	FVector rayDir;

	//드래그 중 Ray
	FVector prevRayWorld;
	FVector currentRayWorld;
	FVector deltaRayWorld;

	//역행렬
	FMatrix inverseProj;
	FMatrix inverseView;

	FVector RayCast();
	void checkCollision();
	bool lineTriangleInter(FVector v0, FVector v1, FVector v2, FVector& outIntersection);
	bool lineMeshIntersection(const UPrimitiveComponent* mesh, FVector& outIntersection);
	void checkFaceCollision();
	FVector TransformVertexToWorld(const FVector& localVertex, const USceneComponent* component);


	void checkGizmo();
	void SetGizmoMode();

	bool prevMouseButtonState = false;
	bool prevSpaceKeyState = false;
};

