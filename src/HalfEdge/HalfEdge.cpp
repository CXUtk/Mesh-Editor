#include "HalfEdge.h"
#include "Vertex.h"
DCEL::HalfEdge::HalfEdge() {
}

DCEL::HalfEdge::HalfEdge(int id) :DCELBase(id) {
    _from = _to = nullptr;
    _next = _twin = nullptr;
    _face = nullptr;
    _edge = nullptr;
}

DCEL::HalfEdge::~HalfEdge() {

}

void DCEL::HalfEdge::DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const {
    throw;
}

glm::vec3 DCEL::HalfEdge::GetCenter() const {
    return (_from->Position + _to->Position) * 0.5f;
}
