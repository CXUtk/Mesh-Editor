#pragma once
#include "DCEL.h"
#include <glm/glm.hpp>

namespace DCEL {
    class Face : public DCELBase {
    public:
        Face();
        Face(int id);

        ~Face() override;

        PHalfEdge HalfEdge() const { return _halfEdge; }
        PHalfEdge& HalfEdge() { return _halfEdge; }

    private:
        PHalfEdge _halfEdge;
    };
}
