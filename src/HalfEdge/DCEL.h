#pragma once

#include "Core/Graphics/Renderer.h"
#include <functional>
namespace DCEL {
    class Vertex;
    class Face;
    class HalfEdge;
    class Edge;

    using PVertex = Vertex*;
    using PHalfEdge = HalfEdge*;
    using PFace = Face*;
    using PEdge = Edge*;
    using const_PVertex = const Vertex*;
    using const_PHalfEdge = const HalfEdge*;
    using const_PFace = const Face*;
    using const_PEdge = const Edge*;


    class DCELBase {
    public:
        DCELBase() :_id(0), _removed(false) {}
        DCELBase(int id) :_id(id), _removed(false) {}
        virtual ~DCELBase() = 0 {}

        // Draw this entity to the scene
        virtual void DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const = 0;

        int GetID() const { return _id; }
        bool ShouldRemove() const { return _removed; }
        void SetRemoveFlag(bool value) { _removed = value; }
    private:
        int _id;
        bool _removed;
    };

    using const_DCELObject = const DCELBase*;
    using EdgeOpPositionFunction = std::function<glm::vec3(PHalfEdge)>;
}
