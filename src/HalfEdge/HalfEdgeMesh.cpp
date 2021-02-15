#include "HalfEdgeMesh.h"
#include <map>

namespace DCEL {
    DCEL::HalfEdgeMesh::HalfEdgeMesh() {
        _totF = _totE = _totHE = _totV = 0;
        _accelStructure = AccelStructure::makeAccelStructure("BVH");
    }

    DCEL::HalfEdgeMesh::~HalfEdgeMesh() {
    }

    void DCEL::HalfEdgeMesh::Build(const TriangleMesh& triangleMesh) {
        std::map<std::pair<int, int>, PHalfEdge> edgeID;
        std::vector<PVertex> V;

        // Read in the vertices
        for (auto& vert : triangleMesh.GetVertices()) {
            V.push_back(newVertex(vert.Position));
        }

        // Construct faces for each indexed faces
        // Detect and assign twin to each half edge
        for (auto& face : triangleMesh.GetFaces()) {
            auto f = newFace();
            PHalfEdge E[3];
            for (int i = 0; i < 3; i++) {
                int fr = face.VertexID[i];
                int to = face.VertexID[(i + 1) % 3];
                E[i] = newHalfEdge(V[fr], V[to]);

                auto itself = edgeID.find({ fr, to });
                if (itself != edgeID.end()) {
                    fprintf(stderr, "Error: Multiple edge detected!\n");
                    fflush(stderr);
                    throw;
                }

                // If reverse edge detected, assign twin
                auto it = edgeID.find({ to, fr });
                if (it != edgeID.end()) {
                    it->second->Twin() = E[i];
                    E[i]->Twin() = it->second;
                    E[i]->Edge() = it->second->Edge();
                    //newEdge(E[i], it->second);
                    edgeID.erase(it);
                }
                else {
                    edgeID[{fr, to}] = E[i];
                    newEdge(E[i], nullptr);
                }
            }
            constructFace(f, E[0], E[1], E[2]);
        }

        if (!edgeID.empty()) {
            fprintf(stdout, "Warning: Boundry edge detected!\n");
        }

        rebuildAccelStructure();
    }

    std::vector<DrawTriangle> HalfEdgeMesh::GetDrawTriangles() const {
        std::vector<DrawTriangle> drawTriangles;
        for (auto& face : _faces) 
            drawTriangles.push_back(face.GetDrawTriangle());
        return drawTriangles;
    }

    std::vector<DrawSegment> HalfEdgeMesh::GetDrawWireFrames() const {
        std::vector<DrawSegment> lines;
        for (auto& edge : _edges)
            lines.push_back(edge.GetDrawSegemnt());
        return lines;
    }

    bool HalfEdgeMesh::RayIntersect(const Ray& ray, HitRecord& hit) const {
        if (!_accelStructure->RayIntersect(ray, hit)) {
            return false;
        }

        // Dected Whether the face or the edge is selected
        const_PFace face = hit.face;
        if (!face) return false;
        const_PHalfEdge E = face->HalfEdge();
        if (hit.barycentric.z < 0.1) {
            hit.hitType = 1;
            hit.edge = E->Edge();
            return true;
        }
        E = E->Next();
        if (hit.barycentric.x < 0.1) {
            hit.hitType = 1;
            hit.edge = E->Edge();
            return true;
        }
        E = E->Next();
        if (hit.barycentric.y < 0.1) {
            hit.hitType = 1;
            hit.edge = E->Edge();
            return true;
        }
        return true;
    }

    PFace DCEL::HalfEdgeMesh::newFace() {
        _totF++;
        _faces.push_back(Face(_totF));
        return &_faces.back();
    }
    PHalfEdge HalfEdgeMesh::newHalfEdge(PVertex a, PVertex b) {
        _totHE++;
        _halfEdges.push_back(HalfEdge(_totHE));
        auto halfEdge = &_halfEdges.back();
        halfEdge->From() = a;
        halfEdge->To() = b;
        return halfEdge;
    }
    PEdge HalfEdgeMesh::newEdge(PHalfEdge a, PHalfEdge b) {
        _totE++;
        _edges.push_back(Edge(_totE));
        auto edge = &_edges.back();
        edge->HalfEdge() = a;
        a->Edge() = edge;
        if(b) b->Edge() = edge;
        return edge;
    }
    PVertex HalfEdgeMesh::newVertex(const glm::vec3& pos) {
        _totV++;
        _vertices.push_back(Vertex(_totV));
        auto vert = &_vertices.back();
        vert->Position = vert->NewPosition = pos;
        return vert;
    }
    void HalfEdgeMesh::constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3) {
        face->HalfEdge() = e1;
        e1->Next() = e2;
        e1->From()->AdjHalfEdge() = e1;

        e2->Next() = e3;
        e2->From()->AdjHalfEdge() = e2;

        e3->Next() = e1;
        e3->From()->AdjHalfEdge() = e3;

        e1->Face() = face, e2->Face() = face, e3->Face() = face;
    }
    void HalfEdgeMesh::rebuildAccelStructure() {
        std::vector<const_PFace> faces;
        for (auto& f : _faces) {
            faces.push_back(&f);
        }
        _accelStructure->Build(faces);
    }
}
