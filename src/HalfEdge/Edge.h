#pragma once
#include "DCEL.h"
#include "Core/Graphics/RenderUtils.h"
#include <glm/glm.hpp>

namespace DCEL {
    class Edge : public DCELBase {
    public:
        Edge();
        Edge(int id);

        ~Edge() override;

        PHalfEdge HalfEdge() const { return _halfEdge; }
        PHalfEdge& HalfEdge() { return _halfEdge; }

        DrawSegment GetDrawSegemnt() const;
    private:
        PHalfEdge _halfEdge;
        bool _isNew;
    };
}
