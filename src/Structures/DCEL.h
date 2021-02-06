#pragma once
#include <list>
#include <glm/glm.hpp>
#include <vector>
#include "Structures/BoundingBox.h"



struct IntersectionInfo;
namespace DCEL {
    struct Vertex;
    struct Face;
    struct HalfEdge;
    struct Edge;

    using PVertex = Vertex*;
    using PHalfEdge = HalfEdge*;
    using PFace = Face*;
    using PEdge = Edge*;
    using const_PVertex = const Vertex*;
    using const_PHalfEdge = const HalfEdge*;
    using const_PFace = const Face*;
    using const_PEdge = const Edge*;

    struct HalfEdge {
        PVertex from, to;
        PHalfEdge twin, next;
        PFace face;
        PEdge edge;
        bool removed;
        int id;

        void changeFrom(PVertex v);
        void changeTo(PVertex v);
        bool isBoundry() const { return !twin; }
    };

    struct Face {
        int id;
        PHalfEdge hEdge;
        bool removed;

        BoundingBox getBoundingBox() const;
        bool rayIntersect(const Ray& ray, IntersectionInfo& info) const;
        glm::vec3 normal() const;
        float area2() const;
    };

    struct Vertex {
        int id;
        bool isNew;
        PHalfEdge hEdge;
        glm::vec3 pos, newPos;
        glm::mat4 quadratic;
        bool removed;

        void computeQuadtratic();
        std::vector<PEdge> getEdges() const;
        std::vector<PFace> getFaces() const;
        glm::vec3 normal() const;
    };

    struct Edge {
        PHalfEdge halfEdge;
        bool isNew;
        bool removed;
        int id;

        bool isBoundry() const { return !halfEdge->twin; }
        float length() const { return glm::length(halfEdge->from->pos - halfEdge->to->pos); }
        glm::vec3 center() const { return (halfEdge->from->pos + halfEdge->to->pos) * 0.5f; }

    };


};


struct IntersectionInfo {
    float t;
    float a, b, c;
    bool isFace;
    union {
        DCEL::const_PEdge edge;
        DCEL::const_PFace face;
    };
};
