#include "InputControl.h"

InputContorl::InputContorl(GLFWwindow* window) : _window(window) {
    _wasMouseDown = _isMouseDown = false;
    _wasMouseRightDown = _isMouseRightDown = false;
    _oldKeysDown = _curKeysDown = 0;
}

void InputContorl::beginInput() {
    _isMouseDown = isMouseLeftDown();
    _isMouseRightDown = isMouseRightDown();
    for (int i = 32; i < 200; i++) {
        _curKeysDown.set(i, isKeyDown(i));
    }
}

void InputContorl::endInput() {
    _wasMouseDown = _isMouseDown;
    _wasMouseRightDown = _isMouseRightDown;
    _oldKeysDown = _curKeysDown;
}

int InputContorl::getScrollValue() const {
    return 0;
}

glm::vec2 InputContorl::getMousePosition() {
    double x, y;
    glfwGetCursorPos(_window, &x, &y);
    return glm::vec2(x, y);
}

bool InputContorl::isKeyDown(int key) {
    return glfwGetKey(_window, key) == GLFW_PRESS;
}
