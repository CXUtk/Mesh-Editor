#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <bitset>
class InputContorl {
public:
    InputContorl(GLFWwindow* window);

    void beginInput();
    void endInput();

    bool getOldMouseDown() const { return _wasMouseDown; }
    bool getCurMouseDown() const { return _isMouseDown; }
    bool getOldMouseRightDown() const { return _wasMouseRightDown; }
    bool getCurMouseRightDown() const { return _isMouseRightDown; }
    bool getWasKeyDown(int key) const { return _oldKeysDown.test(key); }
    bool getIsKeyDown(int key)const { return _curKeysDown.test(key); }
    int getScrollValue() const;
    glm::vec2 getMousePosition();

private:
    bool _wasMouseDown;
    bool _isMouseDown;

    bool _wasMouseRightDown;
    bool _isMouseRightDown;


    std::bitset<256> _curKeysDown;
    std::bitset<256> _oldKeysDown;
    GLFWwindow* _window;

    bool isMouseLeftDown() {
        return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    bool isMouseRightDown() {
        return glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    }


    bool isKeyDown(int key);
};
