#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
    Camera() = default;
    Camera(glm::vec3 eyePos, glm::vec3 lookat, glm::vec3 up, float fov, float aspect, float zNear, float zFar);
    ~Camera() = default;

    glm::mat4 getViewTransform() const;
    glm::mat4 getProjectTransform() const;

    void SetEyePos(glm::vec3 pos) { eyePos = pos; }
    glm::vec3 GetEyePos() const { return eyePos; }

    glm::vec3 GetDir(float x, float y);

private:
    glm::vec3 eyePos, lookAt, up;
    float fov, aspect, zNear, zFar;
};
