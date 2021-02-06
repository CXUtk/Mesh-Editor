#include "HalfEdgeMesh.h"
#include <queue>
#include <glm/gtx/transform.hpp>


namespace DCEL {
    HalfEdgeMesh::HalfEdgeMesh() {
        _totV = 0, _totF = 0;
        _totE = _totHE = 0;
    }
    HalfEdgeMesh::HalfEdgeMesh(const std::vector<glm::vec3>& V, const std::vector<ObjTriangle>& F) {
        build(Mesh(V, F));

        //downSample(1000);
        //for (auto& e : _halfEdges) {
        //    if (!e.twin) {
        //        e.edge->isNew = true;
        //    }
        //}
        //for (auto& v : _vertices) {
        //    assert(v.hEdge);
        //}

        //auto e = _facePool[2].edge;
        //flipEdge(e);
        // flipEdge(_edges.begin()->halfEdge);
        // splitEdge(_edges.begin()->halfEdge);
        //collapseEdge(_edges.begin()->halfEdge, (_edges.begin()->halfEdge->from->pos + _edges.begin()->halfEdge->to->pos) * 0.5f);
        //for (int i = 0; i < 2; i++) upSample();


    }

    HalfEdgeMesh::~HalfEdgeMesh() {

    }

    void HalfEdgeMesh::build(const Mesh& mesh) {
        _vertices.clear();
        _faces.clear();
        _halfEdges.clear();
        _edges.clear();
        _accelStructure = AccelStructure::makeAccelStructure("BVH", 0);

        std::map<std::pair<int, int>, PHalfEdge> halfEdgePairMap;
        std::map<int, PVertex> vertexIdx;
        int idx = 1;
        for (const auto& v : mesh.V) {
            vertexIdx[idx] = newVertex(v);
            idx++;
        }
        for (const auto& f : mesh.F) {
            PHalfEdge edges[3];
            for (int i = 0; i < 3; i++) {
                int a = f.v[i];
                int b = f.v[(i + 1) % 3];
                edges[i] = newHalfEdge(vertexIdx[a], vertexIdx[b]);

                auto pair = halfEdgePairMap.find({ b, a });
                // 如果出现逆边就设置twin
                if (pair == halfEdgePairMap.end()) {
                    halfEdgePairMap[{a, b}] = edges[i];
                }
                else {
                    auto twin = pair->second;
                    newEdge(edges[i], twin);
                    halfEdgePairMap.erase(pair);
                }
            }
            PFace face = newFace();
            constructFace(face, edges[0], edges[1], edges[2]);
        }
        for (auto edge : halfEdgePairMap) {
            newEdgeBroken(edge.second);
        }
        printf("# Faces: %d\n# Vertices: %d\n# Edges: %d\n", getFaceCount(), getVertexCount(), getEdgeCount());
        updateAccelStructure();
    }

    std::vector<Segment> HalfEdgeMesh::getEdges() const {
        std::vector<Segment> res;
        for (auto& e : _edges)
            res.push_back(getDrawEdge(&e));
        return res;
    }

    std::vector<Segment> HalfEdgeMesh::getEdgesMarked() const {
        std::vector<Segment> res;
        bool can = false;
        for (auto& e : _edges) {
            if (e.id == 219 || e.id == 824 || e.id == 61 || e.id == 596) {
                res.push_back(getDrawEdge(&e));
            }
        }
        return res;
    }

    //std::vector<Segment> HalfEdgeMesh::getEdgesMarked() const {
    //    std::vector<Segment> res;
    //    for (auto& e : _edges) {
    //        if (e.isNew)
    //            res.push_back(Segment(e.halfEdge->from->pos, e.halfEdge->to->pos));
    //    }
    //    return res;
    //}

    std::vector<DrawTriangle> HalfEdgeMesh::getTriangles() const {
        std::vector<DrawTriangle> triangles;
        for (auto& f : _faces) {
            triangles.push_back(getDrawFace(&f));
        }
        return triangles;
    }



    void HalfEdgeMesh::save(const std::string& filename) const {
        FILE* file = fopen(filename.c_str(), "w");
        std::map<const Vertex*, int> idMap;
        int id = 1;
        for (auto& v : _vertices) {
            fprintf(file, "v %lf %lf %lf\n", v.pos.x, v.pos.y, v.pos.z);
            idMap[&v] = id;
            id++;
        }
        for (auto& f : _faces) {
            auto A = f.hEdge->from;
            auto B = f.hEdge->to;
            auto C = f.hEdge->next->to;
            fprintf(file, "f %d %d %d\n", idMap[A], idMap[B], idMap[C]);
        }
        fclose(file);
    }

    DrawTriangle HalfEdgeMesh::getDrawFace(const_PFace face) const {
        DrawVertex v[3];
        auto e = face->hEdge;
        for (int i = 0; i < 3; i++) {
            v[i].pos = e->to->pos;
            e = e->next;
        }
        auto normal = glm::normalize(glm::cross(v[1].pos - v[0].pos, v[2].pos - v[0].pos));
        for (int i = 0; i < 3; i++) v[i].normal = normal;

        DrawTriangle tri;
        for (int i = 0; i < 3; i++) {
            tri.V[i] = v[i];
        }
        return tri;
    }

    Segment HalfEdgeMesh::getDrawEdge(const_PEdge edge) const {
        return Segment(edge->halfEdge->from->pos, edge->halfEdge->to->pos);
    }



    bool HalfEdgeMesh::testHitTriangle(const Ray& ray, IntersectionInfo& info) const {
        info.t = std::numeric_limits<float>::infinity();
        info.face = nullptr;
        _accelStructure->rayIntersect(ray, info);
        const_PFace face = info.face;

        if (!face) return false;
        const_PHalfEdge E = face->hEdge;
        if (info.c < 0.1) {
            info.isFace = false;
            info.edge = E->edge;
            return true;
        }
        E = E->next;
        if (info.a < 0.1) {
            info.isFace = false;
            info.edge = E->edge;
            return true;
        }
        E = E->next;
        if (info.b < 0.1) {
            info.isFace = false;
            info.edge = E->edge;
            return true;
        }
        //DrawVertex v[3];
        //auto e = face->hEdge;
        //for (int i = 0; i < 3; i++) {
        //    v[i].pos = e->to->pos;
        //    e = e->next;
        //}
        //auto normal = glm::normalize(glm::cross(v[1].pos - v[0].pos, v[2].pos - v[0].pos));
        //for (int i = 0; i < 3; i++) v[i].normal = normal * 10.f;
        //for (int i = 0; i < 3; i++) {
        //    triangle.V[i] = v[i];
        //}
        return true;
    }

    PVertex DCEL::HalfEdgeMesh::newVertex(const glm::vec3& pos) {
        _vertices.push_back(Vertex());
        PVertex v = &_vertices.back();
        v->hEdge = nullptr;
        //v->iter = std::prev(_vertices.end());
        v->pos = pos;
        v->id = ++_totV;
        v->isNew = false;
        v->newPos = pos;
        v->removed = false;
        return v;
    }

    PFace DCEL::HalfEdgeMesh::newFace() {
        _faces.push_back(Face());
        PFace f = &_faces.back();
        //f->iter = std::prev(_faces.end());
        f->hEdge = nullptr;
        f->id = ++_totF;
        f->removed = false;
        return f;
    }

    PHalfEdge DCEL::HalfEdgeMesh::newHalfEdge(PVertex fr, PVertex to) {
        _halfEdges.push_back(HalfEdge());
        PHalfEdge e = &_halfEdges.back();
        //e->iter = std::prev(_halfEdges.end());
        e->from = fr, e->to = to;
        e->face = nullptr;
        e->next = nullptr;
        e->twin = nullptr;
        e->edge = nullptr;
        e->removed = false;
        e->id = ++_totHE;
        fr->hEdge = e;
        return e;
    }

    PEdge HalfEdgeMesh::newEdge(PHalfEdge a, PHalfEdge b) {
        _edges.push_back(Edge());
        PEdge e = &_edges.back();
        // e->iter = std::prev(_edges.end());
        e->halfEdge = a;
        e->isNew = false;
        e->id = ++_totE;
        a->edge = e, b->edge = e;
        a->twin = b, b->twin = a;
        e->removed = false;
        return e;
    }

    PEdge HalfEdgeMesh::newEdgeBroken(PHalfEdge a) {
        _edges.push_back(Edge());
        PEdge e = &_edges.back();
        //e->iter = std::prev(_edges.end());
        e->halfEdge = a;
        e->id = ++_totE;
        e->isNew = false;
        a->edge = e;
        e->removed = false;
        return e;
    }

    void DCEL::HalfEdgeMesh::constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3) {
        PHalfEdge edges[3];
        edges[0] = e1, edges[1] = e2, edges[2] = e3;
        for (int i = 0; i < 3; i++) {
            edges[i]->next = edges[(i + 1) % 3];
            edges[i]->from->hEdge = edges[i];
            edges[i]->face = face;
        }
        face->hEdge = e1;
    }


    void HalfEdgeMesh::flipEdge(PHalfEdge e) {
        PHalfEdge twin = e->twin;

        if (!twin) {
            return;
        }

        if (e->next->to == twin->next->to) return;

        auto f = e->face;
        auto f2 = twin->face;

        PHalfEdge A = e->next;
        PHalfEdge B = A->next;
        PHalfEdge C = twin->next;
        PHalfEdge D = C->next;

        e->changeFrom(A->to), e->changeTo(C->to);
        twin->changeFrom(C->to), twin->changeTo(A->to);

        constructFace(f, e, D, A);
        constructFace(f2, twin, B, C);
    }


    PVertex HalfEdgeMesh::splitEdge(PHalfEdge e, bool loop) {
        PHalfEdge twin = e->twin;
        auto f = e->face;
        if (!twin) {
            auto pos = (e->from->pos + e->to->pos) * 0.5f;
            auto M = newVertex(pos);
            M->isNew = true;

            auto V = e->next->to;
            auto W = e->to;
            e->changeTo(M);


            auto E1 = newHalfEdge(M, V);
            auto E2 = newHalfEdge(V, M);
            newEdge(E1, E2);

            auto E3 = newHalfEdge(M, W);
            newEdgeBroken(E3);


            auto E4 = e->next;
            constructFace(f, e, E1, e->next->next);
            constructFace(newFace(), E2, E3, E4);
            return M;
        }
        auto f1 = twin->face;

        auto V1 = e->from;
        auto V2 = e->to;
        auto V3 = e->next->to;
        auto V4 = twin->next->to;

        auto pos = (V1->pos + V2->pos) * 0.5f;
        if (loop)
            pos = (V1->pos + V2->pos) * 0.375f + (V3->pos + V4->pos) * 0.125f;
        // pos = glm::normalize(pos);
        auto M = newVertex(pos);
        M->isNew = true;


        auto A = e->next;
        auto B = A->next;
        auto C = twin->next;
        auto D = C->next;

        e->changeFrom(M);
        twin->changeTo(M);

        auto E1 = newHalfEdge(V3, M);
        auto E2 = newHalfEdge(M, V3);
        auto e1 = newEdge(E1, E2);
        e1->isNew = true;

        constructFace(f, e, A, E1);

        auto E3 = newHalfEdge(V1, M);
        auto E4 = newHalfEdge(M, V1);
        auto e2 = newEdge(E3, E4);

        constructFace(f1, E2, B, E3);


        auto E5 = newHalfEdge(V4, M);
        auto E6 = newHalfEdge(M, V4);
        auto e3 = newEdge(E5, E6);
        e3->isNew = true;

        constructFace(newFace(), E4, C, E5);
        constructFace(newFace(), E6, D, twin);

        return M;
    }

    // 逆时针+顺时针把所有不合法的边的起点或者终点设为N
    void collapseAround(PVertex v, PFace f1, PFace f2, PVertex N) {
        auto E = v->hEdge;
        auto F = E->face;
        do {
            if (E->face == f1 || E->face == f2) {
                E = E->next->next;
                E = E->twin;
                if (!E) break;
                continue;
            }
            E->changeFrom(N);
            N->hEdge = E;
            E = E->next->next;
            E->changeTo(N);
            E = E->twin;
            if (!E) break;
        } while (E->face != F);
    }


    // 一定要先判定是否可以坍缩边，并且维护其流形性质
    bool check_valid_collapse(PHalfEdge e) {
        if (e->next->twin->next->to == e->next->next->twin->next->to) return false;
        auto twin = e->twin;
        if (twin && twin->next->twin->next->to == twin->next->next->twin->next->to)return false;
        return true;
    }

    // 这里欠缺边缘三角形退化情况的处理
    PVertex HalfEdgeMesh::collapseEdge(PHalfEdge e, glm::vec3 pos) {
        if (e->edge->id == 57) {
        }
        if (!check_valid_collapse(e))return nullptr;
        assert(e->from != e->to);
        auto twin = e->twin;
        PVertex N = newVertex(pos);
        auto F1 = e->face;
        auto F2 = twin ? twin->face : nullptr;
        collapseAround(e->from, F1, F2, N);
        collapseAround(e->to, F1, F2, N);

        auto L = e->from, R = e->to;

        auto A = e->next->next->twin;
        auto B = e->next->twin;
        makeTwin(A, B);


        // 如果AB边都是边缘，那么删除之后顶点也不存在
        if (!A && !B) {
            deleteVertex(e->next->to);
        }
        else {
            e->next->to->hEdge = B ? B : A->next;
        }


        if (twin) {
            A = twin->next->twin;
            B = twin->next->next->twin;
            makeTwin(A, B);
            if (!A && !B) {
                deleteVertex(twin->next->to);
            }
            else {
                twin->next->to->hEdge = A ? A : B->next;
            }
        }
        deleteFace(F1);
        if (F2) deleteFace(F2);
        deleteVertex(L);
        deleteVertex(R);
        return N;
    }

    void HalfEdgeMesh::deleteVertex(PVertex v) {
        v->removed = true;
        // _vertices.erase(v->iter);
    }


    void HalfEdgeMesh::deleteFace(PFace f) {
        auto e = f->hEdge;
        for (int i = 0; i < 3; i++) {
            auto next = e->next;
            // 如果孪生半边存在且拥有共同的全边，就把孪生半边的全边和孪生边清除
            //if (e->twin && e->twin->edge == e->edge) {
            //    e->twin->removed = true;
            //}
            e->edge->removed = true;
            e->removed = true;
            e = next;
        }
        f->removed = true;
    }

    void HalfEdgeMesh::upSample() {
        for (auto& v : _vertices) {
            v.isNew = false;
            auto edges = v.getEdges();
            float n = edges.size();

            float t = (3.f / 8.f + std::cos(glm::pi<float>() * 2 / n) / 4);
            float u = 3.f / 8.f + t * t;
            glm::vec3 sum(0);
            for (auto e : edges) {
                auto he = e->halfEdge;
                if (e->halfEdge->to == &v) he = e->halfEdge->twin;
                sum += he->to->pos;
            }
            v.newPos = u * v.pos + (1 - u) / n * sum;
        }

        auto it = _edges.begin();
        auto endp = _edges.end();
        int cnt = 0, sz = _edges.size();
        while (it != endp && cnt < sz) {
            auto next = std::next(it);
            splitEdge(it->halfEdge, true);
            it = next;
            ++cnt;
        }

        for (auto& e : _edges) {
            if (e.isNew && (e.halfEdge->from->isNew ^ e.halfEdge->to->isNew)) {
                flipEdge(e.halfEdge);
            }
            if (e.isNew) e.isNew = false;
        }
        for (auto& v : _vertices) {
            v.pos = v.newPos;
        }
        removeFromList();
        printf("Number of faces: %d\n", _faces.size());
        updateAccelStructure();
    }


    struct Node {
        PEdge edge;
        glm::vec3 optimalPos;
        float cost;
        Node() = default;
        Node(PEdge e) :edge(e) {
            glm::mat4 quadratic = glm::zero<glm::mat4>();
            quadratic += e->halfEdge->from->quadratic;
            quadratic += e->halfEdge->to->quadratic;

            glm::mat3 A = quadratic;
            glm::vec3 b = glm::vec3(-quadratic[0][3], -quadratic[1][3], -quadratic[2][3]);
            optimalPos = glm::inverse(A) * b;

            auto v = glm::vec4(optimalPos, 1);
            cost = glm::dot(v, quadratic * v);
        }
        bool operator<(const Node& node) const {
            if (cost == node.cost) {
                return edge < node.edge;
            }
            return cost < node.cost;
        }
    };

    void HalfEdgeMesh::downSample(int maxFaces) {
        for (auto& v : _vertices) {
            v.computeQuadtratic();
        }
        std::set<Node> Q;
        for (auto& e : _edges) {
            Q.insert(Node(&e));
        }

        int cnt = 0;
        while (!Q.empty() && cnt < 50) {
            auto p = *Q.begin();

            auto fr = p.edge->halfEdge->from;
            auto to = p.edge->halfEdge->to;
            auto halfEdge = p.edge->halfEdge;
            auto opt = p.optimalPos;

            auto egss = fr->getEdges();
            for (auto e : egss) {
                auto it = Q.find(e);
                if (it != Q.end()) {
                    Q.erase(it);
                }
            }
            auto egss2 = to->getEdges();
            for (auto e : egss2) {
                Node tmp = e;
                auto it = Q.find(tmp);
                if (it != Q.end()) {
                    Q.erase(it);
                }
            }
            if (Q.empty())break;
            auto V = collapseEdge(halfEdge, opt);
            V->computeQuadtratic();
            //Q.erase(Q.begin());

            auto egs = V->getEdges();
            for (auto e : egs) {
                Q.insert(e);
            }
            cnt++;
        }
        removeFromList();
        printf("Number of faces %d\n", _faces.size());
        updateAccelStructure();
    }

    float square(float x) { return x * x; }
    bool needFlip(const_PEdge edge) {
        auto e = edge->halfEdge;
        auto twin = e->twin;

        auto V1 = e->from;
        auto V2 = e->to;
        auto V3 = e->next->to;
        auto V4 = twin->next->to;

        int D1 = V1->getEdges().size();
        int D2 = V2->getEdges().size();
        int D3 = V3->getEdges().size();
        int D4 = V4->getEdges().size();

        float var = square(D1 - 6) + square(D2 - 6) + square(D3 - 6) + square(D4 - 6);
        float var2 = square(D1 - 6 - 1) + square(D2 - 6 - 1) + square(D3 - 6 + 1) + square(D4 - 6 + 1);
        return var2 < var;
    }

    int cnt = 0;
    void HalfEdgeMesh::reSample() {
        float L = 0;
        for (auto& e : _edges) {
            L += e.length();
        }
        L /= _edges.size();
        cnt++;
        //std::vector<const_PEdge> edges;
        //for (auto& e : _edges) {
        //    edges.push_back(&e);
        //}

        for (auto& e : _edges) {
            auto l = e.length();
            if (l * 3 > 4 * L) {
                splitEdge(e.halfEdge, false);
            }
        }


        for (auto& e : _edges) {
            auto l = e.length();
            if (!e.removed && l * 5 < 4 * L) {

                collapseEdge(e.halfEdge, e.center());
            }
            //for (auto& e1 : _edges) {
            //    if (!e1.removed && e1.halfEdge->edge != &e1) {
            //        assert(false);
            //    }

            //}
        }
        removeFromList();



        //for (auto& e : _edges) {
        //    if (needFlip(&e)) {
        //        flipEdge(e.halfEdge);
        //    }
        //}
        for (auto& v : _vertices) {
            auto es = v.getEdges();
            auto center = glm::vec3(0);
            for (auto e : es) {
                auto he = e->halfEdge;
                if (e->halfEdge->to == &v) he = he->twin;
                center += he->to->pos;
            }
            center /= es.size();

            auto N = v.normal();
            auto V = (v.pos - center);
            V = glm::dot(N, V) * N;
            if (glm::isnan(V.x)) {
                V = glm::vec3(0);
            }
            v.newPos = center + V;
        }
        for (auto& v : _vertices) {
            v.pos = v.newPos;
            if (glm::isnan(v.pos.x)) {
                assert(false);
            }
        }
        printf("Number of faces %d\n", _faces.size());
        updateAccelStructure();
    }

    void HalfEdgeMesh::bulkCheck() {
        for (auto& v : _vertices) {
            printf("Vertex #%d: %x %x\n", v.id, v.hEdge, v.removed);
        }
        for (auto& e : _edges) {
            printf("Edge #%x: %x\n", &e, e.halfEdge);
        }
        for (auto& e : _halfEdges) {
            printf("HalfEdge #%x: %x %x\n", &e, e.twin, e.edge);
        }
        for (auto& f : _faces) {
            printf("Face #%d: %x %x\n", f.id, f.removed, f.hEdge);
        }
    }


    // 把AB两条边连接起来，使得两个相互为对方的twin，
    void HalfEdgeMesh::makeTwin(PHalfEdge A, PHalfEdge B) {
        if (!A && !B) return;
        if (A) {
            A->twin = B;
            A->from->hEdge = A;
        }
        if (B) {
            B->twin = A;
            B->from->hEdge = B;
        }
        if (A && B) {
            newEdge(A, B);
        }
        else {
            newEdgeBroken(A ? A : B);
        }
    }

    void HalfEdgeMesh::updateAccelStructure() {
        std::vector<const_PFace> faces;
        for (auto& f : _faces) {
            faces.push_back(&f);
        }
        _accelStructure->build(faces);
    }

    void HalfEdgeMesh::removeFromList() {
        for (auto it = _edges.begin(); it != _edges.end();) {
            if (it->removed) {
                it = _edges.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _halfEdges.begin(); it != _halfEdges.end();) {
            if (it->removed) {
                it = _halfEdges.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _vertices.begin(); it != _vertices.end();) {
            if (it->removed) {
                it = _vertices.erase(it);
                continue;
            }
            it++;
        }
        for (auto it = _faces.begin(); it != _faces.end();) {
            if (it->removed) {
                it = _faces.erase(it);
                continue;
            }
            it++;
        }

    }


    void HalfEdge::changeFrom(PVertex v) {
        from = v;

    }
    void HalfEdge::changeTo(PVertex v) {
        to = v;
    }


}
