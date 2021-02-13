//#include "DCEL.h"
//#include <set>
//#include <glm/gtx/transform.hpp>
//
//namespace DCEL {
//    Vertex::Vertex() {
//        isNew = false;
//        hEdge = nullptr;
//        pos = newPos = glm::vec3(0);
//        memset(&quadratic, 0, sizeof(glm::mat4));
//        removed = false;
//    }
//    void Vertex::computeQuadtratic() {
//        quadratic = glm::zero<glm::mat4>();
//        auto faces = getFaces();
//        for (auto f : faces) {
//            auto E = f->hEdge;
//            auto A = E->from->pos;
//            auto B = E->to->pos;
//            auto C = E->next->to->pos;
//            auto normal = glm::normalize(glm::cross(B - A, C - A));
//            auto d = -glm::dot(normal, pos);
//            glm::vec4 V = glm::vec4(normal, d);
//
//            quadratic += glm::outerProduct(V, V);
//        }
//    }
//
//
//    std::vector<PEdge> Vertex::getEdges() const {
//        auto E = hEdge;
//        auto F = E->face;
//        std::vector<PEdge> visitedEdge;
//        do {
//            visitedEdge.push_back(E->edge);
//            E = E->next->next;
//            E = E->twin;
//            if (!E) break;
//
//        } while (E->face != F);
//        return visitedEdge;
//    }
//    std::vector<PFace> Vertex::getFaces() const {
//        auto E = hEdge;
//        auto F = E->face;
//        std::vector<PFace> CCW;
//        std::vector<PFace> CW;
//        std::set<PFace> visitedFace;
//
//        do {
//            CCW.push_back(E->face);
//            E = E->next->next;
//            E = E->twin;
//            if (!E) break;
//
//        } while (E->face != F);
//
//        return CCW;
//    }
//    glm::vec3 Vertex::normal() const {
//        glm::vec3 N(0);
//        float totArea = 0;
//        auto faces = getFaces();
//        for (auto& f : faces) {
//            auto area = f->area2();
//            N += area * f->normal();
//            totArea += area;
//        }
//        if (totArea == 0) return N;
//        N /= totArea;
//        if (glm::length(N) == 0)return N;
//        return glm::normalize(N);
//    }
//    Face::Face() {
//        hEdge = nullptr;
//        removed = false;
//    }
//    BoundingBox Face::getBoundingBox() const {
//        glm::vec3 maxx = hEdge->from->pos, minn = hEdge->from->pos;
//        auto e = hEdge->next;
//        for (int i = 1; i < 3; i++) {
//            auto pos = e->from->pos;
//            maxx.x = std::max(maxx.x, pos.x);
//            maxx.y = std::max(maxx.y, pos.y);
//            maxx.z = std::max(maxx.z, pos.z);
//
//            minn.x = std::min(minn.x, pos.x);
//            minn.y = std::min(minn.y, pos.y);
//            minn.z = std::min(minn.z, pos.z);
//            e = e->next;
//        }
//        return BoundingBox(minn, maxx);
//    }
//
//    // 判断光线和这个面是否相交，返回距离和三角形重心坐标
//    bool Face::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
//        glm::vec3 v[3];
//        auto e = hEdge;
//        for (int i = 0; i < 3; i++) {
//            v[i] = e->from->pos;
//            e = e->next;
//        }
//        glm::mat3 A(v[1] - v[0], v[2] - v[0], -ray.dir);
//        auto det = glm::determinant(A);
//        if (isnan(det) || det == 0) return false;
//
//        auto inv = glm::inverse(A);
//        glm::vec3 P = ray.start - v[0];
//        auto res = inv * P;
//        if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
//        info.isFace = true;
//        info.face = this;
//        info.t = res.z;
//        info.a = 1 - res.x - res.y;
//        info.b = res.x;
//        info.c = res.y;
//        return true;
//    }
//    glm::vec3 Face::normal() const {
//        auto e = hEdge;
//        glm::vec3 v[3];
//        for (int i = 0; i < 3; i++) {
//            v[i] = e->to->pos;
//            e = e->next;
//        }
//        return glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
//    }
//    float Face::area2() const {
//        auto e = hEdge;
//        glm::vec3 v[3];
//        for (int i = 0; i < 3; i++) {
//            v[i] = e->to->pos;
//            e = e->next;
//        }
//        return glm::length(glm::cross(v[1] - v[0], v[2] - v[0]));
//    }
//    Edge::Edge() {
//        halfEdge = nullptr;
//        removed = false;
//        isNew = false;
//    }
//};
