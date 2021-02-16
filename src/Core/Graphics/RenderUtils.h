#pragma once
#include <glm/glm.hpp>
#include "Structures/VertexData.h"
struct DrawSegment {
    glm::vec3 A, B;
    DrawSegment() = default;
    DrawSegment(glm::vec3 A, glm::vec3 B) :A(A), B(B) {}
};

struct DrawCircle {
    glm::vec3 Center;
    float Radius;
};

struct DrawTriangle {
    VertexData V[3];
};
