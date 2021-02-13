#pragma once
#include "DCEL.h"
#include <glm/glm.hpp>

namespace DCEL {
    class Vertex : public DCELBase {
    public:
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

        glm::vec3 Position, NewPosition;

    private:
        PHalfEdge _halfEdge;
        glm::mat4 _quadratic;
        bool _isNew;
    };
}
