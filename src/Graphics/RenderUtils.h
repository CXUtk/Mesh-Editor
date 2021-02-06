#pragma once
#include <glm/glm.hpp>
struct Segment {
    glm::vec3 A, B;
    Segment() = default;
    Segment(glm::vec3 A, glm::vec3 B) :A(A), B(B) {}
};


struct DrawVertex {
    glm::vec3 pos, normal;
};

struct DrawTriangle {
    DrawVertex V[3];
};
