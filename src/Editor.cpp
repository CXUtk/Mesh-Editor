#include "Editor.h"
#include <Core/Loaders/ObjLoader.h>
#include <cstdio>
#include <algorithm>

Editor& Editor::GetInstance() {
    static Editor game(800, 600);
    return game;
}

Editor::~Editor() {

}

void Editor::Init() {
    _renderer = std::make_shared<Renderer>();
    ObjLoader loader;
    loader.load("Resources/Scenes/teapot.obj");
    _mesh = std::make_shared<DCEL::HalfEdgeMesh>();
    _mesh->Build(loader.GetMesh());
    _drawTriangles = _mesh->GetDrawTriangles();
    _drawWireFrames = _mesh->GetDrawWireFrames();
    
    _input = std::make_shared<InputContorl>(_window);
    _camera = std::make_shared<CameraController>(5.0f);

    _guiManager = std::make_shared<GUIManager>(_window);
    _guiManager->Init();
}


static double oldTime;
static double oldTime2;
static int cnt;
void Editor::Run() {
    oldTime = oldTime2 = glfwGetTime();
    cnt = 0;
    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        draw();

        glfwSwapBuffers(_window);
        do {
            glfwPollEvents();
        } while (glfwGetTime() - oldTime < 1.0 / 60.0);
        double curTime = glfwGetTime();
        oldTime = curTime;
        if (curTime - oldTime2 > 1.0) {
            cnt = 0;
            oldTime2 = curTime;
        }
        cnt++;
    }
}



static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Editor::Editor(int width, int height) :_width(width), _height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(width, height, "3D Mesh Editor", nullptr, nullptr);
    if (!_window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load glad!\n");
        glfwTerminate();
        return;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


    _selctedDCELObject = nullptr;
}




void Editor::update() {
    _guiManager->Begin();
    _input->beginInput();

    _camera->Run();

    //if (!_input->getWasKeyDown('X') && _input->getIsKeyDown('X')) {
    //    _mesh->upSample();
    //    recalculateMesh();
    //}

    //if (!_input->getWasKeyDown('Z') && _input->getIsKeyDown('Z')) {
    //    _mesh->reSample();
    //    recalculateMesh();
    //}

    //if (!_input->getWasKeyDown('C') && _input->getIsKeyDown('C')) {
    //    _mesh->downSample(300);
    //    recalculateMesh();
    //}
}

void Editor::draw() {
    drawMesh();
    drawSelectedElement();
    //  _renderer->drawLightedTriangles(tFaces, glm::vec3(1, 0, 0), glm::vec3(0, 0, 1));

    _input->endInput();
    _guiManager->End();
    
}

void Editor::drawMesh() {
    _renderer->Begin(_camera->GetCamera()->getProjectTransform(), _camera->GetCamera()->getViewTransform());
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        glDepthFunc(GL_LESS);
        _renderer->DrawLightedTriangles(_drawTriangles, glm::vec3(0.8f), glm::vec3(0, 0, 1), _camera->GetCamera()->GetEyePos());

        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(1.0f, 1.0f);
        _renderer->DrawLines(_drawWireFrames, glm::vec3(1), 1);
        glDisable(GL_POLYGON_OFFSET_LINE);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        //_renderer->drawLines(tEdges, glm::vec3(1, 0, 0), 2);
    }
    _renderer->End();
}


void Editor::drawSelectedElement() {
    auto pos = _input->getMousePosition();
    pos.x /= _width;
    pos.y /= _height;
    pos.y = 1 - pos.y;

    auto dir = _camera->GetCamera()->GetDir(pos.x, pos.y);
    auto ray = Ray(_camera->GetCamera()->GetEyePos(), dir);
    HitRecord hit;

    DCEL::const_PEdge selected = nullptr;
    if (_mesh->RayIntersect(ray, hit)) {
        if (hit.hitType == 0) {
            hit.face->DrawOnScene(_renderer, glm::vec4(1.0f, 0.5f, 0.0f, 0.3f));
            // selected = hit.face;
        }
        else {
            hit.edge->DrawOnScene(_renderer, glm::vec4(1.0f, 0.5f, 0.0f, 0.5f));
            selected = hit.edge;
            // printf("ID: %d, from: %f, to: %f\n", info.edge->id, info.edge->halfEdge->from->pos.x, info.edge->halfEdge->to->pos.x);
        }

    }
    if (!_input->getOldMouseRightDown() && _input->getCurMouseRightDown()) {
        _selctedDCELObject = selected;
    }

    if (_selctedDCELObject != nullptr) {
        _selctedDCELObject->DrawOnScene(_renderer, glm::vec4(1.0f, 0.2f, 0.0f, 0.8f));

        ImGui::Begin("Info");
        ImGui::Text("This element has ID: %d\n", _selctedDCELObject->GetID());
        ImGui::End();

        // 右键可以坍缩边
        if (!_input->getWasKeyDown('Z') && _input->getIsKeyDown('Z')) {
            _mesh->FlipEdge(_selctedDCELObject);
            recalculateMesh();
        }

        if (!_input->getWasKeyDown('X') && _input->getIsKeyDown('X')) {
            _mesh->SplitEdge(_selctedDCELObject);
            recalculateMesh();
        }
    }

}

void Editor::recalculateMesh() {
    _mesh->Recalculate();
    _drawTriangles = _mesh->GetDrawTriangles();
    _drawWireFrames = _mesh->GetDrawWireFrames();
}
