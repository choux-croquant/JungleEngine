#include <windows.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>
#include "URenderer.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

#include "InputManager.h"

#include "UCamera.h"
#include "UCubeComp.h"
#include "FPhysScene.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	InputManager::GetInstance().ProcessMessage(message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WCHAR WindowClass[] = L"JungleWindowClass";
	WCHAR Title[] = L"Game Tech Lab";
	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	RegisterClassW(&wndclass);

	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	URenderer::GetInstance().Initialize(hWnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(URenderer::GetInstance().Device, URenderer::GetInstance().DeviceContext);

	const int targetFPS = 30;
	const double targetFrameTime = 1000.0 / targetFPS;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;

	// Camera Initialize
	UCamera mainCamera(FVector(0.0f, 0.0f, 10.0f), FVector(0.0f, 0.0f, 0.0f), UpVector);

	// TEST CubeComp
	UCubeComp sampleCube1(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	UCubeComp sampleCube2(FVector(2.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	UCubeComp sampleCube3(FVector(0.0f, 2.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	UCubeComp sampleCube4(FVector(0.0f, 0.0f, 2.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	UCubeComp sampleCube5(FVector(-2.0f, -2.0f, -2.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

	FPhysScene physScene(hWnd,&mainCamera);
	physScene.setSampleCube(&sampleCube1);
	physScene.setSampleCube(&sampleCube2);
	physScene.setSampleCube(&sampleCube3);
	physScene.setSampleCube(&sampleCube4);
	physScene.setSampleCube(&sampleCube5);

	while (bIsExit == false)
	{
		QueryPerformanceCounter(&startTime);
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
			
		}
		
		InputManager& input = InputManager::GetInstance();
		mainCamera.MoveCamera(input, 0.016f);
		mainCamera.Update();

		physScene.Update();

		// DirectX 렌더러 루프
		URenderer::GetInstance().Prepare();

		sampleCube1.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCube2.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCube3.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCube4.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCube5.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);

		// IMGUI Section Start
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Jungle Property Window");
		ImGui::Text("Hello Jungle World");

		ImGui::Text("Keyboard State:");
		for (const auto& [key, isPressed] : InputManager::GetInstance().GetKeyStates()) {
			if (isPressed) {
				ImGui::Text("Key %c: Pressed", key);
			}
		}

		// 마우스 버튼 상태 표시
		ImGui::Separator();
		ImGui::Text("Mouse State:");
		ImGui::Text("Position: (%d, %d)", InputManager::GetInstance().GetMousePosition().x, InputManager::GetInstance().GetMousePosition().y);
		ImGui::Text("Left Button: %s", InputManager::GetInstance().IsMouseButtonDown(VK_LBUTTON) ? "Pressed" : "Released");
		ImGui::Text("Right Button: %s", InputManager::GetInstance().IsMouseButtonDown(VK_RBUTTON) ? "Pressed" : "Released");
		ImGui::Text("camera position: %f, %f, %f", mainCamera.RelativeLocation.X, mainCamera.RelativeLocation.Y, mainCamera.RelativeLocation.Z);
		ImGui::Text("camera up direction: %f, %f, %f", mainCamera.upDirection.X, mainCamera.upDirection.Y, mainCamera.upDirection.Z);
		ImGui::Text("camera facing: %f, %f, %f", mainCamera.facing.X, mainCamera.facing.Y, mainCamera.facing.Z);
		ImGui::Text("camera looking at: %f, %f, %f", mainCamera.targetPos.X, mainCamera.targetPos.Y, mainCamera.targetPos.Z);
		ImGui::Text("view Matrix:\n%s", mainCamera.viewMatrix.PrintMatrix().c_str());

		ImGui::End();

		physScene.LogRender();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		// IMGUI Section End

		URenderer::GetInstance().SwapBuffer();

		do {
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		
		} while (elapsedTime < targetFrameTime);

	}
	URenderer::GetInstance().Release();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}