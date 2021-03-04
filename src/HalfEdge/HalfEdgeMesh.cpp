#include "HalfEdgeMesh.h"
#include <map>
#include <glm/gtx/transform.hpp>

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

    void HalfEdgeMesh::Recalculate() {
        do_remove();
        rebuildAccelStructure();
    }

    void HalfEdgeMesh::FlipEdge(const_PEdge edge) {
        PHalfEdge halfEdge = edge->HalfEdge();

        // Stop if it is a boundary edge
        if (!halfEdge->Twin()) return;
        auto twin = halfEdge->Twin();

        auto E1 = halfEdge->Next();
        auto E2 = E1->Next();

        auto E3 = twin->Next();
        auto E4 = E3->Next();

        halfEdge->From() = E3->To(), halfEdge->To() = E1->To();
        twin->From() = E1->To(), twin->To() = E3->To();

        auto F1 = halfEdge->Face();
        auto F2 = twin->Face();
        constructFace(F1, E2, E3, halfEdge);
        constructFace(F2, E4, E1, twin);
    }

    PVertex HalfEdgeMesh::SplitEdge(const_PEdge edge, EdgeOpPositionFunction posFunc) {
        auto halfEdge = edge->HalfEdge();

        PVertex NV = nullptr;
        auto F1 = halfEdge->Face();
        if (!halfEdge->Twin()) {
            NV = newVertex(halfEdge->GetCenter());
            NV->SetNew(true);

            auto E1 = halfEdge->Next();
            auto E2 = E1->Next();

            auto e1 = newHalfEdge(NV, halfEdge->To());
            halfEdge->To() = NV;
            auto e2 = newHalfEdge(NV, E1->To());
            auto e3 = newHalfEdge(E1->To(), NV);
            newEdge(e1, nullptr);
            newEdge(e2, e3);
            e3->Edge()->SetNew(true);

            constructFace(F1, halfEdge, e2, E2);
            constructFace(newFace(), e1, E1, e3);
            return NV;
        }
        NV = newVertex(posFunc(halfEdge));
        NV->SetNew(true);

        auto twin = halfEdge->Twin();
        auto F2 = twin->Face();

        auto E1 = halfEdge->Next();
        auto E2 = E1->Next();
        auto E3 = twin->Next();
        auto E4 = E3->Next();

        halfEdge->To() = NV;
        twin->From() = NV;

        auto e1 = newHalfEdge(NV, E1->To());
        auto e2 = newHalfEdge(E1->To(), NV);
        auto NE1 = newEdge(e1, e2);
        NE1->SetNew(true);

        auto e3 = newHalfEdge(NV, E4->To());
        auto e4 = newHalfEdge(E4->To(), NV);
        newEdge(e3, e4);

        auto e5 = newHalfEdge(NV, E3->To());
        auto e6 = newHalfEdge(E3->To(), NV);
        auto NE2 = newEdge(e5, e6);
        NE2->SetNew(true);

        constructFace(F1, halfEdge, e1, E2);
        constructFace(F2, twin, E3, e6);
        constructFace(newFace(), e4, e5, E4);
        constructFace(newFace(), e3, E1, e2);
        return NV;
    }

    PVertex HalfEdgeMesh::CollapseEdge(PEdge edge, EdgeOpPositionFunction posFunc) {
        if (!edge->SafeToCollapse()) return nullptr;

        auto hEdge = edge->HalfEdge();
        auto twin = hEdge->Twin();
        PVertex V = newVertex(posFunc(hEdge));
        auto fr = hEdge->From(), to = hEdge->To();

        for (auto& hedge : fr->GetAdjHalfEdges()) {
            if (hedge == hEdge || hedge == hEdge->Twin()) continue;
            hedge->From() = V;
            V->AdjHalfEdge() = hedge;
            if (hedge->Twin())
                hedge->Twin()->To() = V;
        }
        for (auto& hedge : to->GetAdjHalfEdges()) {
            if (hedge == hEdge || hedge == hEdge->Twin()) continue;
            hedge->From() = V;
            V->AdjHalfEdge() = hedge;
            if (hedge->Twin())
                hedge->Twin()->To() = V;
        }

        connectEdge(hEdge->Next()->Twin(), hEdge->Next()->Next()->Twin());
        connectEdge(twin->Next()->Twin(), twin->Next()->Next()->Twin());

        removeFace(hEdge->Face());
        removeFace(twin->Face());

        edge->SetRemoveFlag(true);
        fr->SetRemoveFlag(true);
        to->SetRemoveFlag(true);
        return V;
    }

    void HalfEdgeMesh::LoopSubdivision() {
        for (auto& v : _vertices) {
            auto neighbors = v.GetAdjVertices();
            float t = (3.f / 8.f + std::cos(glm::two_pi<float>() / neighbors.size()) / 4);
            float u = 3.f / 8.f + t * t;

            glm::vec3 sum(0);
            for (auto& nv : neighbors) {
                sum += nv->Position;
            }

            v.NewPosition = u * v.Position + (1 - u) * sum / (float)neighbors.size();
        }

        auto loopSplitFunc = [](DCEL::PHalfEdge e) {
            auto v1 = e->From(), v2 = e->To();
            auto v3 = e->Next()->To();
            auto v4 = e->Twin()->Next()->To();
            return (v1->Position + v2->Position) * 0.375f + (v3->Position + v4->Position) * 0.125f;
        };

        auto endp = &(*_edges.rbegin());
        for (auto& e : _edges) {
            SplitEdge(&e, loopSplitFunc);
            if (&e == endp) break;
        }

        for (auto& e : _edges) {
            if (!e.ShouldRemove() && e.IsNew() && e.HalfEdge()->From()->IsNew() != e.HalfEdge()->To()->IsNew()) {
                FlipEdge(&e);
            }
            e.SetNew(false);
        }

        for (auto& v : _vertices) {
            v.Position = v.NewPosition;
            v.SetNew(false);
        }
        Recalculate();
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
        if (b) b->Edge() = edge;
        a->Twin() = b;
        if (b)b->Twin() = a;
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

    void HalfEdgeMesh::connectEdge(PHalfEdge A, PHalfEdge B) {
        A->Edge()->SetRemoveFlag(true);
        B->Edge()->SetRemoveFlag(true);
        A->From()->AdjHalfEdge() = A;
        B->From()->AdjHalfEdge() = B;

        newEdge(A, B);
    }

    void HalfEdgeMesh::removeFace(PFace face) {
        auto e = face->HalfEdge();
        for (int i = 0; i < 3; i++) {
            e->SetRemoveFlag(true);
            e = e->Next();
        }
        face->SetRemoveFlag(true);
    }

    void HalfEdgeMesh::rebuildAccelStructure() {
        std::vector<const_PFace> faces;
        for (auto& f : _faces) {
            faces.push_back(&f);
        }
        _accelStructure->Build(faces);
    }
    void HalfEdgeMesh::do_remove() {
        for (auto it = _edges.begin(); it != _edges.end();) {
            if (it->ShouldRemove()) {
                it = _edges.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _halfEdges.begin(); it != _halfEdges.end();) {
            if (it->ShouldRemove()) {
                it = _halfEdges.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _vertices.begin(); it != _vertices.end();) {
            if (it->ShouldRemove()) {
                it = _vertices.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _faces.begin(); it != _faces.end();) {
            if (it->ShouldRemove()) {
                it = _faces.erase(it);
                continue;
            }
            it++;
        }

    }
}
