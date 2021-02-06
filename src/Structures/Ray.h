#pragma once
#include <glm/glm.hpp>
struct Ray {
    glm::vec3 start, dir;
    Ray() {}
    Ray(glm::vec3 start, glm::vec3 dir) :start(start), dir(dir) {}
};
