#pragma once
#include "DCEL.h"
#include <glm/glm.hpp>
#include <vector>

namespace DCEL {
    class Vertex : public DCELBase {
    public:
        glm::vec3 Position, NewPosition;

        Vertex();
        Vertex(int id);

        ~Vertex() override;

        PHalfEdge AdjHalfEdge() const { return _halfEdge; }
        PHalfEdge& AdjHalfEdge() { return _halfEdge; }

        void MarkNew(bool value) { _isNew = value; }
        bool IsNew() const { return _isNew; }

        glm::mat4 GetQuadratic() const { return _quadratic; }
        void SetQuadratic(const glm::mat4& mat) { _quadratic = mat; }

        glm::vec3 GetNormal() const;

        void DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const override;

        // Enumerate the adjacent edges
        std::vector<PEdge> GetAdjEdges() const;
        std::vector<PHalfEdge> GetAdjHalfEdges() const;
        std::vector<PVertex> GetAdjVertices() const;

    private:
        PHalfEdge _halfEdge;
        glm::mat4 _quadratic;
        bool _isNew;
    };
}
