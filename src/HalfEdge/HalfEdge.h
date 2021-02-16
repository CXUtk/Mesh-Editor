#pragma once
#include "DCEL.h"
#include <glm/glm.hpp>

namespace DCEL {
    class HalfEdge : public DCELBase {
    public:
        HalfEdge();
        HalfEdge(int id);

        ~HalfEdge() override;

        PVertex From() const { return _from; }
        PVertex& From() { return _from; }

        PVertex To() const { return _to; }
        PVertex& To() { return _to; }

        PHalfEdge Twin() const { return _twin; }
        PHalfEdge& Twin() { return _twin; }

        PHalfEdge Next() const { return _next; }
        PHalfEdge& Next() { return _next; }

        PFace Face() const { return _face; }
        PFace& Face() { return _face; }

        PEdge Edge() const { return _edge; }
        PEdge& Edge() { return _edge; }

        void DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const override;

    private:
        PVertex _from, _to;
        PHalfEdge _twin, _next;
        PFace _face;
        PEdge _edge;
    };
}
