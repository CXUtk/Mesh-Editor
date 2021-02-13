#include "HalfEdgeMesh.h"
#include <map>

namespace DCEL {
    DCEL::HalfEdgeMesh::HalfEdgeMesh() {
        _totF = _totE = _totHE = _totV = 0;
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
                    newEdge(E[i], it->second);
                    edgeID.erase(it);
                }
                else {
                    edgeID[{fr, to}] = E[i];
                }
            }
            constructFace(f, E[0], E[1], E[2]);
        }

        if (!edgeID.empty()) {
            fprintf(stdout, "Warning: Boundry edge detected!\n");
        }

    }

    std::vector<DrawTriangle> HalfEdgeMesh::GetDrawTriangles() const {
        std::vector<DrawTriangle> drawTriangles;
        for (auto& face : _faces) {
            DrawTriangle triangle;
            auto edge = face.HalfEdge();
            auto normal = glm::cross(edge->Next()->From()->Position - edge->From()->Position,
                edge->Next()->To()->Position - edge->From()->Position);
            normal = glm::normalize(normal);

            for (int i = 0; i < 3; i++) {
                triangle.V[i].Position = edge->From()->Position;
                triangle.V[i].Normal = normal;
                edge = edge->Next();
            }

            drawTriangles.push_back(triangle);
        }
        return drawTriangles;
    }

    std::vector<Segment> HalfEdgeMesh::GetDrawWireFrames() const {
        std::vector<Segment> lines;
        for (auto& edge : _edges) {
            lines.push_back(Segment(edge.HalfEdge()->From()->Position, edge.HalfEdge()->To()->Position));
        }
        return lines;
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
        b->Edge() = edge;
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
}
