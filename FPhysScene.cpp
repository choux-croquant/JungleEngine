#include "FPhysScene.h"
#include "ImGui/imgui.h"

FPhysScene::FPhysScene(HWND hwnd)
{
	this->hwnd = hwnd;
}

void FPhysScene::Update()
{
	if (input.IsMouseButtonDown(VK_LBUTTON))
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

	}
}

void FPhysScene::LogRender()
{
	ImGui::Begin("FPhysScene Log Window");
	ImGui::Text("FPhysScene Log");
	ImGui::Text("Mouse Screen Pos (%d, %d)", mousePos.x, mousePos.y);
	ImGui::Text("Screen Size (%d, %d)", width, height);
	ImGui::Text("NDC (%f, %f, %f)", ndc.X, ndc.Y,ndc.Z);

	ImGui::End();
}

void FPhysScene::setSampleCube(const UCubeComp& uCubeComp)
{
	cubes.push_back(uCubeComp);
}