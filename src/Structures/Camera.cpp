#include "Camera.h"

Camera::Camera(glm::vec3 eyePos, glm::vec3 lookat, glm::vec3 up, float fov, float aspect, float zNear, float zFar)
    :eyePos(eyePos), lookAt(lookat), up(up), fov(fov), aspect(aspect), zNear(zNear), zFar(zFar) {
}

glm::mat4 Camera::getViewTransform() const {
    return glm::lookAt(eyePos, lookAt, up);
}

glm::mat4 Camera::getProjectTransform() const {
    return glm::perspective(fov, aspect, zNear, zFar);
}

glm::vec3 Camera::getDir(float x, float y) {
    x = x * 2 - 1;
    y = y * 2 - 1;
    auto H = zNear * tan(fov / 2);
    auto W = H * aspect;

    x *= W, y *= H;

    auto look = glm::normalize(lookAt - eyePos);
    auto p = glm::dot(up, look) * look;
    auto top = glm::normalize(up - p);
    auto left = glm::normalize(glm::cross(look, up));
    return glm::normalize(left * x + top * y + look * zNear);
}
