#include "HalfEdge.h"

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
