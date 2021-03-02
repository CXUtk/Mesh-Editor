#include "Edge.h"
#include "HalfEdge.h"
#include "Vertex.h"
#include <set>

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

bool DCEL::Edge::SafeToCollapse() const {
    if (!_halfEdge->Twin()) return false;
    if (!_halfEdge->Next()->Twin() || !_halfEdge->Next()->Next()->Twin()) return false;
    auto twin = _halfEdge->Twin();
    if (!twin->Next()->Twin() || !twin->Next()->Next()->Twin()) return false;

    std::set<PVertex> N1;
    int common = 0;
    auto fr = _halfEdge->From(), to = _halfEdge->To();
    for (auto& v : fr->GetAdjVertices()) {
        N1.insert(v);
    }
    for (auto& v : to->GetAdjVertices()) {
        if (N1.find(v) != N1.end()) {
            common++;
        }
    }
    return common == 2;
}
