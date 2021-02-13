#pragma once
#include <glm/glm.hpp>
#include "Structures/VertexData.h"
struct Segment {
    glm::vec3 A, B;
    Segment() = default;
    Segment(glm::vec3 A, glm::vec3 B) :A(A), B(B) {}
};


struct DrawTriangle {
    VertexData V[3];
};
