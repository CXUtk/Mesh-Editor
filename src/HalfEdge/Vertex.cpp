#include "Vertex.h"
#include <glm/gtx/transform.hpp>

DCEL::Vertex::Vertex(int id) : DCELBase(id),
_halfEdge(nullptr) {
    Position = NewPosition = glm::vec3(0);
    _quadratic = glm::identity<glm::mat4>();
    _isNew = false;
}

DCEL::Vertex::~Vertex() {
}

glm::vec3 DCEL::Vertex::GetNormal() const {
    return glm::vec3();
}
