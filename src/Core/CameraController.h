#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <Structures/Camera.h>
#include <memory>

class CameraController {
public:
    CameraController(float baseDistance);
    
    void Run();
    std::shared_ptr<Camera> GetCamera() const { return _camera; }

private:
    bool _isDragging;
    glm::vec2 _oldOrbitParameter, _curOrbitParameter;
    glm::vec2 _startMousePos;
    float _distance, _factor;

    std::shared_ptr<Camera> _camera;
};
