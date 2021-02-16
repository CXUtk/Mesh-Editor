#include "Edge.h"
#include "HalfEdge.h"
#include "Vertex.h"

DCEL::Edge::Edge() {
}
DCEL::Edge::Edge(int id) : DCELBase(id), _halfEdge(nullptr), _isNew(false) {
}

DCEL::Edge::~Edge() {
}

void DCEL::Edge::DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const {
    std::vector<DrawSegment> vs;
    vs.push_back(GetDrawSegemnt());
    renderer->DrawLines(vs, glm::vec3(color.r, color.g, color.b), 2);
}

DrawSegment DCEL::Edge::GetDrawSegemnt() const {
    return DrawSegment(HalfEdge()->From()->Position, HalfEdge()->To()->Position);
}
