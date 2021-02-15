#include "InputControl.h"

static double scrollValue = 0.0;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollValue += yoffset;
}

InputContorl::InputContorl(GLFWwindow* window) : _window(window) {
    _wasMouseDown = _isMouseDown = false;
    _wasMouseRightDown = _isMouseRightDown = false;
    _oldKeysDown = _curKeysDown = 0;

    glfwSetScrollCallback(window, scroll_callback);
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
    scrollValue = 0.0;
}

int InputContorl::getScrollValue() const {
    return scrollValue;
}

glm::vec2 InputContorl::getMousePosition() {
    double x, y;
    glfwGetCursorPos(_window, &x, &y);
    return glm::vec2(x, y);
}

bool InputContorl::isKeyDown(int key) {
    return glfwGetKey(_window, key) == GLFW_PRESS;
}
