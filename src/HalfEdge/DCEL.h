#pragma once

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

        int GetID() const { return _id; }
        bool ShouldRemove() const { return _removed; }
        void SetRemoveFlag(bool value) { _removed = value; }
    private:
        int _id;
        bool _removed;
    };
}
