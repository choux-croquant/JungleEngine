#pragma once
#include <Windows.h>
#include <unordered_map>
#include "ImGui/imgui.h"

class UInputManager {
public:
    static UInputManager& GetInstance() {
        static UInputManager instance;
        return instance;
    }

    // 키가 눌려있는지 여부 확인
    bool IsKeyDown(unsigned char key) const {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    // KeyState getter
    std::unordered_map<unsigned char, bool> GetKeyStates() const {
        return keyStates;
    }

    // 마우스 좌표 가져오기
    POINT GetMousePosition() const {
        return mousePosition;
    }

    // 마우스 이동량 가져오기
    POINT GetMouseDelta() const {
        return mouseDelta;
    }

    // 마우스 버튼 상태 확인
    bool IsMouseButtonDown(int button) const {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second;
    }

    // 윈도우 메시지 처리 (윈도우 프로시저에서 호출해야 함)
    void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {

        if (ImGui::GetCurrentContext() != nullptr) {
            ImGuiIO& io = ImGui::GetIO();
            if (io.WantCaptureMouse && (message == WM_MOUSEMOVE ||
                message == WM_LBUTTONDOWN || message == WM_LBUTTONUP ||
                message == WM_RBUTTONDOWN || message == WM_RBUTTONUP))
                return;
            if (io.WantCaptureKeyboard && message == WM_KEYDOWN)
                return;
        }

        switch (message) {
        case WM_KEYDOWN:
            keyStates[wParam] = true;
            break;
        case WM_KEYUP:
            keyStates[wParam] = false;
            break;
        case WM_MOUSEMOVE:
            // 이전 마우스 위치를 저장
            previousMousePosition = mousePosition;
            // 현재 마우스 위치 업데이트
            mousePosition.x = LOWORD(lParam);
            mousePosition.y = HIWORD(lParam);
            // 마우스 이동량 계산
            mouseDelta.x = mousePosition.x - previousMousePosition.x;
            mouseDelta.y = mousePosition.y - previousMousePosition.y;
            break;
        case WM_LBUTTONDOWN:
            mouseButtonStates[VK_LBUTTON] = true;
            break;
        case WM_LBUTTONUP:
            mouseButtonStates[VK_LBUTTON] = false;
            break;
        case WM_RBUTTONDOWN:
            mouseButtonStates[VK_RBUTTON] = true;
            break;
        case WM_RBUTTONUP:
            mouseButtonStates[VK_RBUTTON] = false;
            break;
        }
    }

    void Update() {
        mouseDelta = { 0, 0 };
    }

private:
    std::unordered_map<unsigned char, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    POINT mousePosition = { 0, 0 };
    POINT previousMousePosition = { 0, 0 }; // 이전 마우스 위치
    POINT mouseDelta = { 0, 0 }; // 마우스 이동량

    UInputManager() {}
    ~UInputManager() {}
    UInputManager(const UInputManager&) = delete;
    UInputManager& operator=(const UInputManager&) = delete;
};
