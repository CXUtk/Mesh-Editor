#include "Edge.h"
#include "HalfEdge.h"
#include "Vertex.h"

DCEL::Edge::Edge() {
}
DCEL::Edge::Edge(int id) : DCELBase(id), _halfEdge(nullptr), _isNew(false) {
}

DCEL::Edge::~Edge() {
}

DrawSegment DCEL::Edge::GetDrawSegemnt() const {
    return DrawSegment(HalfEdge()->From()->Position, HalfEdge()->To()->Position);
}
