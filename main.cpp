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

#include "USphereComp.h"
#include "UCylinderComp.h"
#include "UConeComp.h"
#include "UWorldAxis.h"

#include "ULog.h"
#include "UMemory.h"
#include "ScenePropertyWindow.h"
#include "UWorld.h"
#include "ULevel.h"
#include <random>

#include "SceneSaveManager.h"

constexpr float BaseWindowWidth = 1024.0f;
constexpr float BaseWindowHeight = 1024.0f;

std::random_device rd;
std::mt19937 gen(rd());

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
	case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			URenderer::GetInstance().OnResize(width, height);

			if (ImGui::GetCurrentContext() != nullptr)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)width, (float)height);
			}
		}
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
		CW_USEDEFAULT, CW_USEDEFAULT, BaseWindowWidth, BaseWindowHeight,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	URenderer::GetInstance().Initialize(hWnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(URenderer::GetInstance().Device, URenderer::GetInstance().DeviceContext);

	const int targetFPS = 60;
	const double targetFrameTime = 1000.0 / targetFPS;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;

	// Camera Initialize
	UCamera mainCamera(FVector(-10.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f));
	mainCamera.Update();

	// Guide Axis
	UWorldAxis worldAxis(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

	UCubeComp sampleCube(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	USphereComp sampleSphere(FVector(2.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	UConeComp sampleCone(FVector(0.0f, 2.0f, 0.0f), FVector(PI / 2, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

	USceneComponent group(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(0.2f, 0.2f, 0.2f));

	
	UCylinderComp sampleCylinderX(FVector(2.5f, 0.0f, 0.0f), FVector(0.0f, PI / 2, 0.0f), FVector(0.5f, 0.5f, 3.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), true);
	UConeComp sampleConeX(FVector(6.0f, 0.0f, 0.0f), FVector(0.0f, PI / 2, 0.0f), FVector(1.0f, 1.0f, 1.0f), FVector4(1.0f, 0.0f, 0.0f, 1.0f), true);
	UCylinderComp sampleCylinderY(FVector(0.0f, 2.5f, 0.0f), FVector(PI / 2, 0.0f, 0.0f), FVector(0.5f, 0.5f, 3.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f), true);
	UConeComp sampleConeY(FVector(0.0f, 6.0f, 0.0f), FVector(PI / 2, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), FVector4(0.0f, 1.0f, 0.0f, 1.0f), true);
	UCylinderComp sampleCylinderZ(FVector(0.0f, 0.0f, 2.5f), FVector(0.0f, 0.0f, 0.0f), FVector(0.5f, 0.5f, 3.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), true);
	UConeComp sampleConeZ(FVector(0.0f, 0.0f, 6.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), FVector4(0.0f, 0.0f, 1.0f, 1.0f), true);

	sampleConeX.AttachTo(&group);
	sampleCylinderX.AttachTo(&group);
	sampleConeY.AttachTo(&group);
	sampleCylinderY.AttachTo(&group);
	sampleConeZ.AttachTo(&group);
	sampleCylinderZ.AttachTo(&group);


	FPhysScene physScene(hWnd,&mainCamera);
	physScene.SetPrimitive(&sampleCube);
	physScene.SetPrimitive(&sampleSphere);
	physScene.SetPrimitive(&sampleCone);
	//physScene.SetPrimitive(&sampleCylinder);
	//physScene.SetPrimitive(&sampleCone);
	physScene.SetGizmo(&sampleCylinderX, &sampleConeX,GizmoAxis::X);
	physScene.SetGizmo(&sampleCylinderY, &sampleConeY, GizmoAxis::Y);
	physScene.SetGizmo(&sampleCylinderZ, &sampleConeZ, GizmoAxis::Z);
	physScene.SetGizmoGroup(&group);
	
	ScenePropertyWindow scenePropertyWindow(mainCamera);

	int selectedPrimitive = 0;
	int primitiveSpawnNum = 0;
	std::uniform_real_distribution<float> dis(-5.0f, 5.0f);
	ULevel* currLevel = new ULevel();
	currLevel->OnPrimitiveSpawned.push_back([&physScene](UPrimitiveComponent* newPrimitive) {
		physScene.SetPrimitive(newPrimitive);
	});

	SceneSaveManager sceneSaveManager;
	char saveFileName[10] = "Default";


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


		// DirectX 렌더러 루프
		URenderer::GetInstance().Prepare();
		worldAxis.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		
		sampleCube.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleSphere.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);

		sampleCone.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);

		sampleConeX.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCylinderX.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleConeY.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCylinderY.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleConeZ.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		sampleCylinderZ.Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		
		#pragma region ImGui
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Jungle Control Panel");
		ImGui::Text("Hello Jungle World!");
		ImGui::Text("FPS %.1f (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Separator();

		// Spawn Primitives
		const char* options[] = { "Cube", "Sphere", "Cylinder", "Cone" };
		if (ImGui::Combo("Primitive", &selectedPrimitive, options, IM_ARRAYSIZE(options)))
		{
			UE_LOG(SpawnOption, Log, "%d", selectedPrimitive);
		}
		if (ImGui::Button("Spawn"))
		{
			for (int i = 0; i < primitiveSpawnNum; i++)
			{
				FVector randomPos(dis(gen), dis(gen), dis(gen));
				EPrimitiveType type = static_cast<EPrimitiveType>(selectedPrimitive);
				currLevel->SpawnPrimitiveByType(type, randomPos, FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
			}
		}
		ImGui::SameLine();
		ImGui::InputInt("Number of spawn", &primitiveSpawnNum);
		TArray<UPrimitiveComponent*> primitives = currLevel->GetPrimitives();
		for (const auto& primitive : primitives)
		{
			primitive->Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
		}
		ImGui::Separator();

		physScene.Update();

		//씬 저장
		ImGui::InputText("Scene Name", saveFileName, 10);
		if(ImGui::Button("New Scene")) {
			delete currLevel;
			currLevel = new ULevel();
			sceneSaveManager.SceneData.clear();
		}
		if (ImGui::Button("Save Scene")) {
			sceneSaveManager.NextUUID = UEngineStatics::NextUUID;
			TArray<UPrimitiveComponent*> primitives;
			primitives = currLevel->GetPrimitives();
			if (primitives.size() > 0) {
				for (int i = 0; i < primitives.size(); i++) {
					std::string TypeName = primitives[i]->GetTypeName();
					SceneSaveManager::PrimitiveData data;
					data.UUID = primitives[i]->GetUUID();
					data.Location = primitives[i]->GetLocation();
					data.Rotation = primitives[i]->GetRotation();
					data.Scale = primitives[i]->GetScale();
					data.Type = primitives[i]->GetTypeName();
					sceneSaveManager.SceneData.push_back(data);
				}
				json SavedJson = sceneSaveManager.toJson();
				sceneSaveManager.Save(saveFileName);
			}
		}
		if (ImGui::Button("Load Scene")) {
			if (sceneSaveManager.Load(saveFileName)) {
				delete currLevel;
				currLevel = new ULevel();
				for (int i = 0; i < sceneSaveManager.SceneData.size(); i++) {
					EPrimitiveType type = UPrimitiveComponent::GetType(sceneSaveManager.SceneData[i].Type);
					currLevel->SpawnPrimitiveByType(type, sceneSaveManager.SceneData[i].Location, sceneSaveManager.SceneData[i].Rotation, sceneSaveManager.SceneData[i].Scale);
				}
				for (const auto& primitive : currLevel->GetPrimitives())
				{
					primitive->Render(mainCamera.viewMatrix, mainCamera.projectionMatrix);
				}
			}
			else {
				MessageBoxW(hWnd, L"No such file!", L"Error", MB_OK);
			}
		}
		ImGui::Separator();

		//카메라 속성창
		scenePropertyWindow.Draw();
		mainCamera.MoveCamera(0.016f);
		mainCamera.Update();
		InputManager::GetInstance().Update();
		scenePropertyWindow.UpdateCamera();

		ImGui::Separator();
		ImGui::Text("Keyboard State:");
		for (const auto& [key, isPressed] : InputManager::GetInstance().GetKeyStates()) {
			if (isPressed) {
				ImGui::Text("Key %c: Pressed", key);
			}
		}

		// 마우스 버튼 상태 표시
		ImGui::Separator();
		ImGui::Checkbox("hasRotated", &scenePropertyWindow.hasRotated);
		ImGui::Text("Mouse State:");
		ImGui::Text("Position: (%d, %d)", InputManager::GetInstance().GetMousePosition().x, InputManager::GetInstance().GetMousePosition().y);
		ImGui::Text("Left Button: %s", InputManager::GetInstance().IsMouseButtonDown(VK_LBUTTON) ? "Pressed" : "Released");
		ImGui::Text("Right Button: %s", InputManager::GetInstance().IsMouseButtonDown(VK_RBUTTON) ? "Pressed" : "Released");
		ImGui::Text("camera position: %f, %f, %f", mainCamera.RelativeLocation.X, mainCamera.RelativeLocation.Y, mainCamera.RelativeLocation.Z);
		ImGui::Text("camera up direction: %f, %f, %f", mainCamera.upDirection.X, mainCamera.upDirection.Y, mainCamera.upDirection.Z);
		ImGui::Text("camera facing: %f, %f, %f", mainCamera.facing.X, mainCamera.facing.Y, mainCamera.facing.Z);
		ImGui::Text("camera rotation: %f, %f, %f", RadtoDeg(mainCamera.RelativeRotation.X), RadtoDeg(mainCamera.RelativeRotation.Y), RadtoDeg(mainCamera.RelativeRotation.Z));
		ImGui::Text("view Matrix:\n%s", mainCamera.viewMatrix.PrintMatrix().c_str());

		if (currLevel->GetPrimitives().size() != 0) {
			ImGui::Text("obejct UUID  : %d", currLevel->GetPrimitives()[0]->GetUUID());
		}

		// Heap Memory
		ImGui::Text("Total Bytes  : %d", UMemory::GetInstance().GetTotalAllocationBytes());
		ImGui::Text("Total Count  : %d", UMemory::GetInstance().GetTotalAllocationCount());
		ImGui::End();

		physScene.LogRender();
		physScene.PickedObjPropertyRender();
    
		// UE_LOG
		ULog::DrawLogWindow();
		if (InputManager::GetInstance().IsKeyDown('H'))
			UE_LOG(LogTemp, Log, "Hello World %d", 2025);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion

		URenderer::GetInstance().SwapBuffer();
		do {
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		
		} while (elapsedTime < targetFrameTime);

	}
	delete currLevel;
	URenderer::GetInstance().Release();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}