#include "FPhysScene.h"
#include "ImGui/imgui.h"

FPhysScene::FPhysScene(HWND hwnd, UCamera* camera)
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

void FPhysScene::Update()
{
	bool currentMouseButtonState = input.IsMouseButtonDown(VK_LBUTTON);

	if (!prevMouseButtonState && input.IsMouseButtonDown(VK_LBUTTON))
	{
		//클릭 한번
		prevRayWorld = RayCast();
		//checkCollision();
		checkFaceCollision();
		checkGizmo();
	}
	else if (prevMouseButtonState && input.IsMouseButtonDown(VK_LBUTTON))
	{
		//드래그
		currentRayWorld = RayCast();
		deltaRayWorld = currentRayWorld - prevRayWorld;
		if (closestHitObject != nullptr)
		{
			switch (CurrentGizmo.gizmoAxis)
			{
			case GizmoAxis::X:
				closestHitObject->RelativeLocation.X += deltaRayWorld.X;
				break;
			case GizmoAxis::Y:
				closestHitObject->RelativeLocation.Y += deltaRayWorld.Y;
				break;
			case GizmoAxis::Z:
				closestHitObject->RelativeLocation.Z += deltaRayWorld.Z;
				break;
			default:
				break;
			}
		}
		prevRayWorld = currentRayWorld;
	}

	

	prevMouseButtonState = currentMouseButtonState;

	if (rayCollision)
	{
		m_gizmoGroup->AttachTo(closestHitObject);
		//closestHitObject->RelativeLocation + deltaRayWorld;
	}
	else if (!rayCollision&&!isGizmoClicked)
	{
		m_gizmoGroup->DetachFromParent();
	}
}

void FPhysScene::LogRender()
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
	ImGui::End();
}

void FPhysScene::PickedObjPropertyRender()
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

FVector FPhysScene::RayCast()
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

	FMatrix projI = camera->projectionMatrix.Inverse();
	FMatrix viewI = camera->viewMatrix.Inverse();

	FVector4 rayViewW = projI.TransformVector(ndc);

	if (rayViewW.W != 0)
	{
		rayView.X = -rayViewW.X / rayViewW.W;
		rayView.Y = -rayViewW.Y / rayViewW.W;
		rayView.Z = -rayViewW.Z / rayViewW.W;
		rayView.W = -1.0f;
	}

	FVector4 rayWorld4 = viewI.TransformVector(rayView);

	rayWorld.X = rayWorld4.X;
	rayWorld.Y = rayWorld4.Y;
	rayWorld.Z = rayWorld4.Z;

	rayDir = rayWorld - camera->RelativeLocation;
	rayDir.Normalize();

	return rayWorld;
}

void FPhysScene::checkCollision()
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

void FPhysScene::checkFaceCollision()
{
	FVector rayOrigin = camera->RelativeLocation;
	UPrimitiveComponent* closestObject = nullptr;
	float min_t = FLT_MAX;
	bool Hit = false;
	for (UPrimitiveComponent* cube : primitives)
	{
		if (cube)
		{
			FVector MeshIntersection;
			if (lineMeshIntersection(cube, MeshIntersection))
			{
				float T = (rayOrigin - MeshIntersection).Length();
				if (T < min_t)
				{
					min_t = T;
					closestObject = cube;
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
	else
	{
		rayCollision = false;
		closestHitObject = nullptr;
	}
}

bool FPhysScene::lineTriangleInter(FVector v0, FVector v1, FVector v2, FVector& outIntersection)
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

bool FPhysScene::lineMeshIntersection(const UPrimitiveComponent* mesh, FVector& outIntersection)
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


FVector FPhysScene::TransformVertexToWorld(const FVector& localVertex, const USceneComponent* component)
{
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
	FVector transformedVertex = ModelMatrix.TransformVector(localVertex);
	return transformedVertex;
}

void FPhysScene::checkGizmo()
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

	if (closestGizmo.gizmoCone &&
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

void FPhysScene::SetPrimitive(UPrimitiveComponent* uCubeComp)
{
	primitives.push_back(uCubeComp);
}

void FPhysScene::SetGizmo(UPrimitiveComponent* cylinder, UPrimitiveComponent* cone, GizmoAxis gizmoAxis)
{
	Gizmo gizmo;
	gizmo.gizmoCylinder = cylinder;
	gizmo.gizmoCone = cone;
	gizmo.gizmoAxis = gizmoAxis;

	gizmos.push_back(gizmo);
}

void FPhysScene::SetGizmoGroup(USceneComponent* gizmoGroup)
{
	m_gizmoGroup = gizmoGroup;
}
