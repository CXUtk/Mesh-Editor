#pragma once
#include <glm/glm.hpp>
#include "HalfEdge/DCEL.h"
#include <algorithm>
struct HitRecord {
    float t;
    glm::vec3 barycentric;
    int hitType;
    union {
        DCEL::const_PEdge edge;
        DCEL::const_PFace face;
        DCEL::const_PVertex vertex;
    };

    HitRecord() {
        t = std::numeric_limits<float>::infinity();
        barycentric = glm::vec3(0);
        hitType = 0;
        edge = nullptr;
    }
};

struct Ray {
    glm::vec3 start, dir;
    Ray() {}
    Ray(glm::vec3 start, glm::vec3 dir) :start(start), dir(dir) {}
};
