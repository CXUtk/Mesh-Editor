#include "Edge.h"
DCEL::Edge::Edge(int id) : DCELBase(id), _halfEdge(nullptr), _isNew(false) {
}

DCEL::Edge::~Edge() {
}
