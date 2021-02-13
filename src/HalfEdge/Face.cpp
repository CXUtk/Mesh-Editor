#include "Face.h"

DCEL::Face::Face(int id) : DCELBase(id) {
    _halfEdge = nullptr;
}

DCEL::Face::~Face() {
}
