#include "UPhysScene.h"
#include "ImGui/imgui.h"
#include "ULog.h"

UPhysScene::UPhysScene(HWND hwnd, UCamera* camera)
{
	this->hwnd = hwnd;
	this->camera = camera;

	mousePos.x = 0;
	mousePos.y = 0;

	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;
}

void UPhysScene::Update()
{
	SetGizmoMode();
	bool currentMouseButtonState = input.IsMouseButtonDown(VK_LBUTTON);

	if (!prevMouseButtonState && input.IsMouseButtonDown(VK_LBUTTON))
	{
		//클릭 한번
		prevRayWorld = RayCast();
		//checkCollision();
		checkGizmo();
		checkFaceCollision();
	}
	else if (prevMouseButtonState && input.IsMouseButtonDown(VK_LBUTTON))
	{
		//드래그
		currentRayWorld = RayCast();
		deltaRayWorld = currentRayWorld - prevRayWorld;

		float len;
		if (closestHitObject != nullptr)
		{
			closestHitObject->bIsClicked = true;
			FMatrix rotation =
			FMatrix::RotateX(closestHitObject->RelativeRotation.X) *
			FMatrix::RotateY(closestHitObject->RelativeRotation.Y) *
			FMatrix::RotateZ(closestHitObject->RelativeRotation.Z);
			FVector axisX(rotation.M[0][0], rotation.M[1][0], rotation.M[2][0]);
			FVector axisY(rotation.M[0][1], rotation.M[1][1], rotation.M[2][1]);
			FVector axisZ(rotation.M[0][2], rotation.M[1][2], rotation.M[2][2]);
			if (CurrentGizmo.gizmoCone != nullptr) {
				CurrentGizmo.gizmoCone->bIsClicked = true;
				CurrentGizmo.gizmoCylinder->bIsClicked = true;
			}
			switch (currentGizmoMode)
			{
			case GizmoMode::TRANSLATE:
				// 이동 모드
				switch (CurrentGizmo.gizmoAxis)
				{
				case GizmoAxis::X:
					len = deltaRayWorld.Dot(axisX);
					closestHitObject->RelativeLocation += axisX.Normalize() * len * 0.1f;
					break;
				case GizmoAxis::Y:
					len = deltaRayWorld.Dot(axisY);
					closestHitObject->RelativeLocation += axisY.Normalize() * len * 0.1f;
					break;
				case GizmoAxis::Z:
					len = deltaRayWorld.Dot(axisZ);
					closestHitObject->RelativeLocation += axisZ.Normalize() * len * 0.1f;
					break;
				}
			break;

			case GizmoMode::ROTATE:
				// 회전 모드
				switch (CurrentGizmo.gizmoAxis)
				{
				case GizmoAxis::X:
					len = deltaRayWorld.Dot(axisX);
					closestHitObject->RelativeRotation.X += len * 5.0f;
					m_gizmoGroup->RelativeRotation = closestHitObject->RelativeRotation;
					break;
				case GizmoAxis::Y:
					len = deltaRayWorld.Dot(axisY);
					closestHitObject->RelativeRotation.Y += len * 5.0f;
					m_gizmoGroup->RelativeRotation = closestHitObject->RelativeRotation;
					break;
				case GizmoAxis::Z:
					len = deltaRayWorld.Dot(axisZ);
					closestHitObject->RelativeRotation.Z += len * 5.0f;
					m_gizmoGroup->RelativeRotation = closestHitObject->RelativeRotation;
					break;
				}
			break;

			case GizmoMode::SCALE:
				// 크기 조절 모드
				switch (CurrentGizmo.gizmoAxis)
				{
				case GizmoAxis::X:
					len = deltaRayWorld.Dot(axisX);
					if (closestHitObject->RelativeScale3D.X + len * 0.1f < 0.001f) return;
					closestHitObject->RelativeScale3D.X += len * 0.1f;
					break;
				case GizmoAxis::Y:
					len = deltaRayWorld.Dot(axisY);
					if (closestHitObject->RelativeScale3D.Y + len * 0.1f < 0.001f) return;
					closestHitObject->RelativeScale3D.Y += len * 0.1f;
					break;
				case GizmoAxis::Z:
					len = deltaRayWorld.Dot(axisZ);
					if (closestHitObject->RelativeScale3D.Z + len * 0.1f < 0.001f) return;
					closestHitObject->RelativeScale3D.Z += len * 0.1f;
					break;
				}
			break;

			default:
			break;
		}
	}
		for (UPrimitiveComponent* primitive : primitives) {
			if (closestHitObject != primitive) {
				primitive->bIsClicked = false;
			}
		}
		for (Gizmo gizmo : gizmos) {
			if (CurrentGizmo.gizmoCone != gizmo.gizmoCone) {
				gizmo.gizmoCone->bIsClicked = false;
				gizmo.gizmoCylinder->bIsClicked = false;
			}
		}
		prevRayWorld = currentRayWorld;
	}

	prevMouseButtonState = currentMouseButtonState;

	if (rayCollision)
	{
		//m_gizmoGroup->AttachTo(closestHitObject);
		m_gizmoGroup->RelativeLocation = closestHitObject->RelativeLocation;
	}
	else if (!rayCollision && !isGizmoClicked)
	{
		//m_gizmoGroup->DetachFromParent();
		m_gizmoGroup->RelativeLocation = FVector(1000.0f, 0.0f, 0.0f);
	}
}

void UPhysScene::LogRender()
{
	ImGui::Begin("FPhysScene Log Window");
	ImGui::Text("FPhysScene Log");
	ImGui::Text("Mouse Screen Pos (%d, %d)", mousePos.x, mousePos.y);
	ImGui::Text("Screen Size (%d, %d)", width, height);
	ImGui::Text("NDC (%f, %f, %f)", ndc.X, ndc.Y, ndc.Z);
	ImGui::Text("rayView (%f, %f, %f, %f)", rayView.X, rayView.Y, rayView.Z, rayView.W);
	ImGui::Text("rayWorld (%f, %f, %f)", rayWorld.X, rayWorld.Y, rayWorld.Z);

	if (rayCollision)
	{
		ImGui::Text("Ray Collision Object ID : %d", closestHitObject->UUID);
		ImGui::Text("Ray Collision Object Type : %s", closestHitObject->GetTypeName().c_str());
	}
	else
	{
		ImGui::Text("Ray Collision Object None");
	}

	if (isGizmoClicked)
	{
		ImGui::Text("Clicked Gizmo Axis : %d", CurrentGizmo.gizmoAxis);
		ImGui::Text("Delta Ray World: (%f, %f, %f)",
			deltaRayWorld.X, deltaRayWorld.Y, deltaRayWorld.Z);
	}
	else
	{
		ImGui::Text("Clicked Gizmo None");
	}

	ImGui::Text("inverse projection\n%s", inverseProj.PrintMatrix().c_str());
	ImGui::Text("view Matrix:\n%s", camera->viewMatrix.PrintMatrix().c_str());
	ImGui::Text("inverse view\n%s", inverseView.PrintMatrix().c_str());
	ImGui::End();
}

void UPhysScene::PickedObjPropertyRender()
{
	static UPrimitiveComponent* lastSelectedObject = nullptr; // 이전 선택된 오브젝트 저장

	if (closestHitObject != nullptr) {
		lastSelectedObject = closestHitObject;
	}

	if (lastSelectedObject != nullptr) {
		FVector& T = lastSelectedObject->RelativeLocation;
		FVector& R = lastSelectedObject->RelativeRotation;
		FVector& S = lastSelectedObject->RelativeScale3D;

		ImGui::Begin("Object Property");
		ImGui::PushItemWidth(50);

		// Translation
		ImGui::InputFloat("##T_X", &T.X, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##T_Y", &T.Y, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##T_Z", &T.Z, 0, 0); ImGui::SameLine();
		ImGui::Text("Translation");

		// Rotation
		ImGui::InputFloat("##R_X", &R.X, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##R_Y", &R.Y, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##R_Z", &R.Z, 0, 0); ImGui::SameLine();
		ImGui::Text("Rotation");

		// Scale
		ImGui::InputFloat("##S_X", &S.X, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##S_Y", &S.Y, 0, 0); ImGui::SameLine();
		ImGui::InputFloat("##S_Z", &S.Z, 0, 0); ImGui::SameLine();
		ImGui::Text("Scale");

		ImGui::PopItemWidth();
		ImGui::End();
	}
}

TArray<UPrimitiveComponent*> UPhysScene::GetPrimitives()
{
	return primitives;
}

FVector UPhysScene::RayCast()
{
	mousePos = input.GetMousePosition();

	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	float ndcX = (2.0f * float(mousePos.x)) / float(width) - 1.0f;
	float ndcY = 1.0f - (2.0f * float(mousePos.y)) / float(height);
	float ndcZ = 1.0f;

	ndc.X = ndcX;
	ndc.Y = ndcY;
	ndc.Z = ndcZ;
	ndc.W = 1.0f;

	FMatrix projI = camera->projectionMatrix.InverseGaussJordan();
	inverseProj = projI;
	FMatrix viewI = camera->viewMatrix.InverseGaussJordan();
	inverseView = viewI;

	FVector4 rayViewW = projI.TransformVector(ndc);

	if (rayViewW.W != 0)
	{
		rayView.X = rayViewW.X / rayViewW.W;
		rayView.Y = rayViewW.Y / rayViewW.W;
		rayView.Z = rayViewW.Z / rayViewW.W;
		rayView.W = 1.0f;
	}

	FVector4 rayWorld4 = viewI.TransformVector(rayView);

	rayWorld.X = rayWorld4.X;
	rayWorld.Y = rayWorld4.Y;
	rayWorld.Z = rayWorld4.Z;

	rayDir = rayWorld - camera->RelativeLocation;
	rayDir.Normalize();

	return rayWorld;
}

void UPhysScene::checkCollision()
{
	FVector rayOrigin = camera->RelativeLocation;
	float min_t = FLT_MAX;
	UPrimitiveComponent* closestObject = nullptr;

	for (UPrimitiveComponent* cube : primitives)
	{
		if (cube)
		{
			FVector cubeCenter = cube->RelativeLocation;

			FVector V = cubeCenter - rayOrigin;
			float t_closest = V.Dot(rayDir);

			FVector P = rayOrigin + rayDir * t_closest;

			float d2 = (P - cubeCenter).Dot(P - cubeCenter);
			float r2 = 1.0f;
			if (d2 < r2 && t_closest < min_t)
			{
				min_t = t_closest;
				closestObject = cube;
			}
		}
	}

	if (closestObject)
	{
		rayCollision = true;
		closestHitObject = closestObject;
	}
	else
	{
		rayCollision = false;
		closestHitObject = nullptr;
	}
}

void UPhysScene::checkFaceCollision()
{
	FVector rayOrigin = camera->RelativeLocation;
	UPrimitiveComponent* closestObject = nullptr;
	float min_t = FLT_MAX;
	bool Hit = false;
	for (UPrimitiveComponent* primitive : primitives)
	{
		if (primitive)
		{
			FVector MeshIntersection;
			if (lineMeshIntersection(primitive, MeshIntersection))
			{
				float T = (rayOrigin - MeshIntersection).Length();
				if (T < min_t)
				{
					min_t = T;
					closestObject = primitive;
					//OutIntersection = MeshIntersection
					Hit = true;
				}
			}
		}
	}

	if (closestObject)
	{
		rayCollision = true;
		closestHitObject = closestObject;
	}
	else if(!isGizmoClicked)
	{
		rayCollision = false;
		closestHitObject = nullptr;
	}
}

bool UPhysScene::lineTriangleInter(FVector v0, FVector v1, FVector v2, FVector& outIntersection)
{
	//v0, v1, v2 는 clockwise 순서
	FVector rayOrigin = camera->RelativeLocation;

	FVector e1 = v1 - v0;
	FVector e2 = v2 - v0;

	FVector normal = e1.Cross(e2);
	normal.Normalize();

	float NdotRayDir = normal.Dot(rayDir);
	if (abs(NdotRayDir) < 1e-6f) return false;

	float d = normal.Dot(v0);
	float t = (d - normal.Dot(rayOrigin)) / NdotRayDir;

	if (t < 0.0f) return false;

	FVector P = rayOrigin + rayDir * t;

	FVector edge0 = v1 - v0;
	FVector edge1 = v2 - v1;
	FVector edge2 = v0 - v2;

	FVector C0 = P - v0;
	FVector C1 = P - v1;
	FVector C2 = P - v2;

	FVector EdgeCrosC0 = edge0.Cross(C0);
	FVector EdgeCrosC1 = edge1.Cross(C1);
	FVector EdgeCrosC2 = edge2.Cross(C2);

	if (normal.Dot(EdgeCrosC0) < 0) return false;
	if (normal.Dot(EdgeCrosC1) < 0) return false;
	if (normal.Dot(EdgeCrosC2) < 0) return false;

	outIntersection = P;
	return true;
}

bool UPhysScene::lineMeshIntersection(const UPrimitiveComponent* mesh, FVector& outIntersection)
{
	FVector rayOrigin = camera->RelativeLocation;

	float minT = FLT_MAX;
	bool hit = false;
	const MeshData meshData = mesh->meshData;
	const TArray<FVertexSimple>& Vertices = meshData.Vertices;
	const TArray<uint32>& Indices = meshData.Indices;

	for (int i = 0; i < Indices.size(); i += 3)
	{
		FVector V0 = Vertices[Indices[i]].Position;
		FVector V1 = Vertices[Indices[i + 1]].Position;
		FVector V2 = Vertices[Indices[i + 2]].Position;

		V0 = TransformVertexToWorld(V0, mesh);
		V1 = TransformVertexToWorld(V1, mesh);
		V2 = TransformVertexToWorld(V2, mesh);

		FVector intersection;
		if (lineTriangleInter(V0, V1, V2, intersection))
		{
			float T = (rayOrigin - intersection).Length();
			if (T < minT)
			{
				minT = T;
				outIntersection = intersection;
				hit = true;
			}
		}
	}


	return hit;
}


FVector UPhysScene::TransformVertexToWorld(const FVector& localVertex, const USceneComponent* component)
{
	/*
	FVector WorldPos = component->GetWorldLocation();
	FVector WorldRotation = component->GetWorldRotation();
	FVector WorldScale = component->GetWorldScale3D();

	FMatrix T = FMatrix::Translate(WorldPos);
	FMatrix Rx = FMatrix::RotateX(WorldRotation.X);
	FMatrix Ry = FMatrix::RotateY(WorldRotation.Y);
	FMatrix Rz = FMatrix::RotateZ(WorldRotation.Z);
	FMatrix R = Rz * Ry * Rx;
	FMatrix S = FMatrix::Scale(WorldScale);

	FMatrix ModelMatrix = T * R * S;
	*/
	FMatrix ModelMatrix = component->GetWorldTransform();
	FVector transformedVertex = ModelMatrix.TransformVector(localVertex);
	return transformedVertex;
}

void UPhysScene::checkGizmo()
{
	FVector rayOrigin = camera->RelativeLocation;
	Gizmo closestGizmo;
	closestGizmo.gizmoCone = nullptr;
	closestGizmo.gizmoCylinder = nullptr;

	float min_t = FLT_MAX;
	for (Gizmo gizmo : gizmos)
	{
		UPrimitiveComponent* gizmoCylinder = gizmo.gizmoCylinder;
		UPrimitiveComponent* gizmoCone = gizmo.gizmoCone;
		GizmoAxis axis = gizmo.gizmoAxis;
		if (gizmoCylinder)
		{
			FVector MeshIntersection;
			if (lineMeshIntersection(gizmoCylinder, MeshIntersection))
			{
				float T = (rayOrigin - MeshIntersection).Length();
				if (T < min_t)
				{
					min_t = T;
					closestGizmo.gizmoCylinder = gizmoCylinder;
					closestGizmo.gizmoCone = gizmoCone;
					closestGizmo.gizmoAxis = axis;
				}
			}

		}
		if (gizmoCone)
		{
			FVector MeshIntersection;
			if (lineMeshIntersection(gizmoCone, MeshIntersection))
			{
				float T = (rayOrigin - MeshIntersection).Length();
				if (T < min_t)
				{
					min_t = T;
					closestGizmo.gizmoCylinder = gizmoCylinder;
					closestGizmo.gizmoCone = gizmoCone;
					closestGizmo.gizmoAxis = axis;
				}
			}

		}
	}

	if (closestGizmo.gizmoCone ||
		closestGizmo.gizmoCylinder)
	{
		CurrentGizmo = closestGizmo;
		isGizmoClicked = true;
	}
	else
	{
		CurrentGizmo.gizmoCone = nullptr;
		CurrentGizmo.gizmoCylinder = nullptr;
		isGizmoClicked = false;
	}
}

void UPhysScene::SetGizmoMode() {
	UInputManager& input = UInputManager::GetInstance();

	bool currentSpaceKeyState = input.IsKeyDown(' ');

	if (!prevSpaceKeyState && currentSpaceKeyState) {
		switch (currentGizmoMode)
		{
		case GizmoMode::TRANSLATE:
			currentGizmoMode = GizmoMode::ROTATE;
			break;
		case GizmoMode::ROTATE:
			currentGizmoMode = GizmoMode::SCALE;
			break;
		case GizmoMode::SCALE:
			currentGizmoMode = GizmoMode::TRANSLATE;
			break;
		default:
			break;
		}
	}

	prevSpaceKeyState = currentSpaceKeyState;
}

void UPhysScene::SetPrimitive(UPrimitiveComponent* uCubeComp)
{
	primitives.push_back(uCubeComp);
}

void UPhysScene::SetGizmo(UPrimitiveComponent* cylinder, UPrimitiveComponent* cone, GizmoAxis gizmoAxis)
{
	Gizmo gizmo;
	gizmo.gizmoCylinder = cylinder;
	gizmo.gizmoCone = cone;
	gizmo.gizmoAxis = gizmoAxis;

	gizmos.push_back(gizmo);
}

void UPhysScene::SetGizmoGroup(USceneComponent* gizmoGroup)
{
	m_gizmoGroup = gizmoGroup;
}
