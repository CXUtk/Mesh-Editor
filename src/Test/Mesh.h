#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Loaders/ObjTriangle.h"
struct Mesh {
    std::vector<glm::vec3> V;
    std::vector<ObjTriangle> F;
    Mesh() {}
    Mesh(const std::vector<glm::vec3>& V, const std::vector<ObjTriangle>& F) : V(V), F(F) {

    }
};
