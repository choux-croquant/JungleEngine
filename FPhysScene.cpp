#include "FPhysScene.h"
#include "ImGui/imgui.h"

FPhysScene::FPhysScene(HWND hwnd, const UCamera* camera)
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
	if (input.IsMouseButtonDown(VK_LBUTTON))
	{
		RayCast();
		checkCollision();
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
		ImGui::Text("Ray Collision Object ID %d", closestHitObject->UUID);
	}
	else
	{
		ImGui::Text("Ray Collision Object None");
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

void FPhysScene::RayCast()
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

}

void FPhysScene::checkCollision()
{
	FVector rayOrigin = camera->RelativeLocation;
	float min_t = FLT_MAX;
	UPrimitiveComponent* closestObject = nullptr;

	for (UPrimitiveComponent* cube : cubes)
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

void FPhysScene::setSampleCube(UCubeComp* uCubeComp)
{
	cubes.push_back(uCubeComp);
}