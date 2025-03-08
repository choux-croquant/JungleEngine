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
	}
}

void FPhysScene::LogRender()
{
	ImGui::Begin("FPhysScene Log Window");
	ImGui::Text("FPhysScene Log");
	ImGui::Text("Mouse Screen Pos (%d, %d)", mousePos.x, mousePos.y);
	ImGui::Text("Screen Size (%d, %d)", width, height);

	ImGui::End();
}

void FPhysScene::setSampleCube(const UCubeComp& uCubeComp)
{
	cubes.push_back(uCubeComp);
}