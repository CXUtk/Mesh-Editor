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

        void DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const override;

        DrawSegment GetDrawSegemnt() const;

        bool IsNew() const { return _isNew; }
        void SetNew(bool v) { _isNew = v; }

    private:
        PHalfEdge _halfEdge;
        bool _isNew;
    };
}
