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

    // Ű�� �����ִ��� ���� Ȯ��
    bool IsKeyDown(unsigned char key) const {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    // KeyState getter
    std::unordered_map<unsigned char, bool> GetKeyStates() const {
        return keyStates;
    }

    // ���콺 ��ǥ ��������
    POINT GetMousePosition() const {
        return mousePosition;
    }

    // ���콺 �̵��� ��������
    POINT GetMouseDelta() const {
        return mouseDelta;
    }

    // ���콺 ��ư ���� Ȯ��
    bool IsMouseButtonDown(int button) const {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second;
    }

    // ������ �޽��� ó�� (������ ���ν������� ȣ���ؾ� ��)
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
            // ���� ���콺 ��ġ�� ����
            previousMousePosition = mousePosition;
            // ���� ���콺 ��ġ ������Ʈ
            mousePosition.x = LOWORD(lParam);
            mousePosition.y = HIWORD(lParam);
            // ���콺 �̵��� ���
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
    POINT previousMousePosition = { 0, 0 }; // ���� ���콺 ��ġ
    POINT mouseDelta = { 0, 0 }; // ���콺 �̵���

    UInputManager() {}
    ~UInputManager() {}
    UInputManager(const UInputManager&) = delete;
    UInputManager& operator=(const UInputManager&) = delete;
};
