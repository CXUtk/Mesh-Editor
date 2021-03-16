#include "CameraController.h"
#include <Editor.h>

CameraController::CameraController(float baseDistance) : _distance(baseDistance), _factor(0.5) {
    _isDragging = false;
    _curOrbitParameter = _oldOrbitParameter = glm::vec2(0);
    _startMousePos = glm::vec2(0);            
    _distance = _factor * _factor * 20 + 0.5f;

    auto width = Editor::GetInstance().GetWidth();
    auto height = Editor::GetInstance().GetHeight();
    constexpr float fov = glm::pi<float>() / 2;
    float aspect = width / (float)height;
    _camera = std::shared_ptr<Camera>(new Camera(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), fov, aspect, .2f, 40.f));
}

void CameraController::Run() {
    auto input = Editor::GetInstance().GetInputController();
    auto height = Editor::GetInstance().GetHeight();


    // Êó±ê¿ØÖÆÍÏ×§
    //if (!input->getOldMouseDown() && input->getCurMouseDown()) {
    //    _isDragging = true;
    //    _startMousePos = input->getMousePosition();
    //    _startMousePos.y = height - _startMousePos.y;
    //}
    //if (_isDragging) {
    //    auto pos = input->getMousePosition();
    //    pos.y = height - pos.y;

    //    glm::vec2 moved = pos - _startMousePos;
    //    moved *= 0.01f;

    //    _curOrbitParameter = _oldOrbitParameter + 0.01f * glm::vec2(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
    //    auto pi = glm::pi<float>();
    //    _curOrbitParameter.x = std::max(-pi, std::min(pi, _curOrbitParameter.x));
    //    _curOrbitParameter.y = std::max(-pi / 2 + 0.001f, std::min(pi / 2 - 0.001f, _curOrbitParameter.y));
    //}



    if (!ImGui::GetIO().WantCaptureMouse) {
        if (ImGui::GetIO().MouseClicked[0]) {
            _isDragging = true;
        }
        if (_isDragging) {
            _curOrbitParameter = _oldOrbitParameter + 0.01f * glm::vec2(ImGui::GetIO().MouseDelta.x, -ImGui::GetIO().MouseDelta.y);
            auto pi = glm::pi<float>();
            _curOrbitParameter.x = std::max(-pi, std::min(pi, _curOrbitParameter.x));
            _curOrbitParameter.y = std::max(-pi / 2 + 0.001f, std::min(pi / 2 - 0.001f, _curOrbitParameter.y));
            //if (input->getOldMouseDown() && !input->getCurMouseDown()) {
            //    _isDragging = false;
            //    _oldOrbitParameter = _curOrbitParameter;
            //}
            _oldOrbitParameter = _curOrbitParameter;
        }
        if (ImGui::GetIO().MouseReleased[0]) {
            _isDragging = false;
        }

        // ¹öÂÖ¿ØÖÆÀëÔ­µã¾àÀë
        if (input->getScrollValue() != 0) {
            _factor += input->getScrollValue() * -0.01f;
            _factor = std::max(0.f, std::min(1.0f, _factor));
            _distance = _factor * _factor * 20 + 0.5f;
        }

    }
    float r2 = std::cos(_curOrbitParameter.y);
    _camera->SetEyePos(_distance * glm::vec3(-r2 * std::sin(_curOrbitParameter.x),
        -std::sin(_curOrbitParameter.y), r2 * std::cos(_curOrbitParameter.x)));
}
