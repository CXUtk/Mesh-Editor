#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"

DCEL::Face::Face() {
}

DCEL::Face::Face(int id) : DCELBase(id) {
    _halfEdge = nullptr;
}

DCEL::Face::~Face() {
}

BoundingBox DCEL::Face::GetBoundingBox() const {
    glm::vec3 maxx = _halfEdge->From()->Position, minn = _halfEdge->From()->Position;
    auto e = _halfEdge->Next();
    for (int i = 1; i < 3; i++) {
        auto pos = e->From()->Position;
        maxx.x = std::max(maxx.x, pos.x);
        maxx.y = std::max(maxx.y, pos.y);
        maxx.z = std::max(maxx.z, pos.z);

        minn.x = std::min(minn.x, pos.x);
        minn.y = std::min(minn.y, pos.y);
        minn.z = std::min(minn.z, pos.z);
        e = e->Next();
    }
    return BoundingBox(minn, maxx);
}

bool DCEL::Face::RayInterset(const Ray& ray, HitRecord* hit) const {
    glm::vec3 v[3];
    auto e = _halfEdge;
    for (int i = 0; i < 3; i++) {
        v[i] = e->From()->Position;
        e = e->Next();
    }
    glm::mat3 A(v[1] - v[0], v[2] - v[0], -ray.dir);
    auto det = glm::determinant(A);
    if (isnan(det) || det == 0) return false;

    auto inv = glm::inverse(A);
    glm::vec3 P = ray.start - v[0];
    auto res = inv * P;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
    hit->hitType = 0;
    hit->face = this;
    hit->barycentric = glm::vec3(1 - res.x - res.y, res.x, res.y);
    hit->t = res.z;
    return true;
}

void DCEL::Face::DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<DrawTriangle> vs;
    vs.push_back(GetDrawTriangle());
    renderer->DrawTriangles(vs, color);
    glDisable(GL_BLEND);
}

DrawTriangle DCEL::Face::GetDrawTriangle() const {
    DrawTriangle triangle;
    auto edge = HalfEdge();
    auto normal = glm::cross(edge->Next()->From()->Position - edge->From()->Position,
        edge->Next()->To()->Position - edge->From()->Position);
    normal = glm::normalize(normal);

    for (int i = 0; i < 3; i++) {
        triangle.V[i].Position = edge->From()->Position;
        triangle.V[i].Normal = normal;
        edge = edge->Next();
    }
    return triangle;
}
