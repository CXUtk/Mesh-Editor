#include "Editor.h"
#include "Loaders/ObjLoader.h"
#include <cstdio>
#include <algorithm>

Editor& Editor::GetInstance() {
    static Editor game(800, 600);
    return game;
}



Editor::~Editor() {

}
ObjLoader loader;
void Editor::init() {
    _isDragging = false;
    _curOrbitParameter = _oldOrbitParameter = glm::vec2(0);


    _renderer = std::make_shared<Renderer>();

    loader.load("Resources/Scenes/gd32.obj");
    _mesh = std::make_shared<DCEL::HalfEdgeMesh>();
    _mesh->Build(loader.GetMesh());
    _drawTriangles = _mesh->GetDrawTriangles();
    _drawWireFrames = _mesh->GetDrawWireFrames();
    recalculateMesh();
    //_mesh->upSample();
    //_mesh->upSample();

    //TestMeshes test;
    //_mesh->build(test._meshes[0]);

    constexpr float fov = glm::pi<float>() / 2;
    float aspect = _width / (float)_height;
    _camera = std::shared_ptr<Camera>(new Camera(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), fov, aspect, 1.f, 100.f));
    _input = std::make_shared<InputContorl>(_window);

    _guiManager = std::make_shared<GUIManager>(_window);
    _guiManager->Init();

}


static double oldTime;
static double oldTime2;
static int cnt;
void Editor::run() {
    oldTime = oldTime2 = glfwGetTime();
    cnt = 0;
    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        draw();

        glfwSwapBuffers(_window);
        while (glfwGetTime() - oldTime < 1.0 / 60.0) {
            glfwPollEvents();
        }
        double curTime = glfwGetTime();
        oldTime = curTime;
        if (curTime - oldTime2 > 1.0) {
            //printf("FPS: %d\n", cnt);
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

    init();

}


glm::vec2 startMousePos;

void Editor::update() {
    _input->beginInput();

    if (!_input->getOldMouseDown() && _input->getCurMouseDown()) {
        _isDragging = true;
        startMousePos = _input->getMousePosition();
        startMousePos.y = _height - startMousePos.y;
    }
    if (_isDragging) {
        auto pos = _input->getMousePosition();
        pos.y = _height - pos.y;

        glm::vec2 moved = pos - startMousePos;
        moved *= 0.01f;

        _curOrbitParameter = _oldOrbitParameter + moved;
        auto pi = glm::pi<float>();
        _curOrbitParameter.x = std::max(-pi, std::min(pi, _curOrbitParameter.x));
        _curOrbitParameter.y = std::max(-pi / 2 + 0.001f, std::min(pi / 2 - 0.001f, _curOrbitParameter.y));

    }
    if (_input->getOldMouseDown() && !_input->getCurMouseDown()) {
        _isDragging = false;
        _oldOrbitParameter = _curOrbitParameter;
    }

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



    float r = 3.f;
    float r2 = std::cos(_curOrbitParameter.y);
    _camera->SetEyePos(r * glm::vec3(-r2 * std::sin(_curOrbitParameter.x),
        -std::sin(_curOrbitParameter.y), r2 * std::cos(_curOrbitParameter.x)));
}

void Editor::draw() {
    _renderer->begin(_camera->getProjectTransform(), _camera->getViewTransform());
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        glDepthFunc(GL_LESS);
        _renderer->drawLightedTriangles(_drawTriangles, glm::vec3(0.8), glm::vec3(0, 0, 1), _camera->GetEyePos());

        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(1.0f, 1.0f);
        _renderer->drawLines(_drawWireFrames, glm::vec3(1), 1);
        glDisable(GL_POLYGON_OFFSET_LINE);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        //_renderer->drawLines(tEdges, glm::vec3(1, 0, 0), 2);
    }
    _renderer->end();

    //  _renderer->drawLightedTriangles(tFaces, glm::vec3(1, 0, 0), glm::vec3(0, 0, 1));
    auto pos = _input->getMousePosition();
    pos.x /= _width;
    pos.y /= _height;
    pos.y = 1 - pos.y;

    //auto dir = _camera->getDir(pos.x, pos.y);
    //auto ray = Ray(_camera->GetEyePos(), dir);
    //IntersectionInfo info;
    //if (_mesh->testHitTriangle(ray, info)) {
    //    if (info.isFace) {
    //        std::vector<DrawTriangle> vs;
    //        vs.push_back(_mesh->getDrawFace(info.face));
    //        _renderer->drawTriangles(vs, glm::vec3(1, 0, 0));
    //    }
    //    else {
    //        std::vector<Segment> vs;
    //        vs.push_back(_mesh->getDrawEdge(info.edge));
    //        _renderer->drawLines(vs, glm::vec3(1, 1, 0), 2);
    //        // printf("ID: %d, from: %f, to: %f\n", info.edge->id, info.edge->halfEdge->from->pos.x, info.edge->halfEdge->to->pos.x);
    //    }

    //    // 右键可以坍缩边
    //    if (!_input->getOldMouseRightDown() && _input->getCurMouseRightDown() && !info.isFace) {
    //        _mesh->collapseEdge(info.edge->halfEdge, (info.edge->halfEdge->from->pos + info.edge->halfEdge->to->pos) * 0.5f);
    //        // printf("# Faces: %d\n# Vertices: %d\n# Edges: %d\n", _mesh->getFaceCount(), _mesh->getVertexCount(), _mesh->getEdgeCount());
    //        //_mesh->bulkCheck();
    //        recalculateMesh();

    //    }
    //}


    _input->endInput();
}

void Editor::recalculateMesh() {
    //_mesh->recalculate();
    //_drawTriangles = _mesh->getTriangles();
    //_drawWireFrames = _mesh->getEdges();

}
