#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Graphics/Renderer.h"
#include "HalfEdge/HalfEdgeMesh.h"
#include "Structures/CameraController.h"
#include "Input/InputControl.h"
#include "UI/GUIManager.h"
#include <memory>

class Editor {
public:
    static Editor& GetInstance();
    ~Editor();


    void Init();
    void Run();
    std::shared_ptr<InputContorl> GetInputController() const { return _input; }
    int GetHeight() const { return _height; }
    int GetWidth() const { return _width; }

private:
    // 构造函数private声明，确保单例
    Editor(int width, int height);

    int _width, _height;
    GLFWwindow* _window;

    std::shared_ptr<DCEL::HalfEdgeMesh> _mesh;
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<CameraController> _camera;
    std::shared_ptr<InputContorl> _input;
    std::shared_ptr<GUIManager> _guiManager;

    std::vector<Segment> _drawWireFrames;
    std::vector<DrawTriangle> _drawTriangles;

    void update();
    void draw();


    void recalculateMesh();
};
