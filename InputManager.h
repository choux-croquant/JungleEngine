#pragma once
#include <Windows.h>
#include <unordered_map>


class InputManager {
public:
    static InputManager& GetInstance() {
        static InputManager instance;
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

    // ���콺 ��ư ���� Ȯ��
    bool IsMouseButtonDown(int button) const {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second;
    }

    // ������ �޽��� ó�� (������ ���ν������� ȣ���ؾ� ��)
    void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_KEYDOWN:
            keyStates[wParam] = true;
            break;
        case WM_KEYUP:
            keyStates[wParam] = false;
            break;
        case WM_MOUSEMOVE:
            mousePosition.x = LOWORD(lParam);
            mousePosition.y = HIWORD(lParam);
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

private:
    std::unordered_map<unsigned char, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    POINT mousePosition = { 0, 0 };

    InputManager() {}
    ~InputManager() {}
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
};
