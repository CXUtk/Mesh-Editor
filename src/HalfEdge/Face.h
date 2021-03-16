#pragma once
#include "DCEL.h"
#include <Structures/Ray.h>
#include <Structures/BoundingBox.h>
#include <glm/glm.hpp>
#include "Core/Graphics/RenderUtils.h"

namespace DCEL {
    class Face : public DCELBase {
    public:
        Face();
        Face(int id);

        ~Face() override;

        PHalfEdge HalfEdge() const { return _halfEdge; }
        PHalfEdge& HalfEdge() { return _halfEdge; }

        BoundingBox GetBoundingBox() const;
        bool RayInterset(const Ray& ray, HitRecord* hit) const;

        void DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const override;

        glm::vec3 GetNormal() const;

        DrawTriangle GetDrawTriangle() const;

    private:
        PHalfEdge _halfEdge;
    };
}
