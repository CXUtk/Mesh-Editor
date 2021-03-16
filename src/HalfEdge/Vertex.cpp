#include "Vertex.h"
#include <glm/gtx/transform.hpp>
#include <HalfEdge/HalfEdge.h>
#include <HalfEdge/Edge.h>
#include <HalfEdge/Face.h>
namespace DCEL {
    DCEL::Vertex::Vertex() {
    }

    DCEL::Vertex::Vertex(int id) : DCELBase(id), _halfEdge(nullptr) {
        Position = NewPosition = glm::vec3(0);
        _quadratic = glm::identity<glm::mat4>();
        _isNew = false;
    }

    DCEL::Vertex::~Vertex() {
    }

    void Vertex::ComputeQuadratic() {
        _quadratic = glm::zero<glm::mat4>();
        for (auto& he : GetAdjHalfEdges()) {
            auto N = he->Face()->GetNormal();
            N = glm::normalize(N);
            auto d = -glm::dot(N, he->Face()->HalfEdge()->From()->Position);
            auto V = glm::vec4(N, d);
            _quadratic += glm::outerProduct(V, V);
        }
    }

    glm::vec3 DCEL::Vertex::GetNormal() const {
        glm::vec3 normal = glm::vec3(0);
        for (auto& he : GetAdjHalfEdges()) {
            normal += he->Face()->GetNormal();
        }
        return glm::normalize(normal);
    }

    void DCEL::Vertex::DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const {
        // renderer->DrawCircle();
    }

    std::vector<PEdge> DCEL::Vertex::GetAdjEdges() const {
        std::vector<PEdge> edges;
        PHalfEdge hedge = _halfEdge;
        do {
            edges.push_back(hedge->Edge());
            if (!hedge->Twin()) break;
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return edges;
    }

    std::vector<PHalfEdge> DCEL::Vertex::GetAdjHalfEdges() const {
        std::vector<PHalfEdge> edges;
        PHalfEdge hedge = _halfEdge;
        do {
            edges.push_back(hedge);
            if (!hedge->Twin()) break;
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return edges;
    }
    std::vector<PVertex> Vertex::GetAdjVertices() const {
        std::vector<PVertex> vertices;
        PHalfEdge hedge = _halfEdge;
        do {
            vertices.push_back(hedge->To());
            if (!hedge->Twin()) break;
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return vertices;
    }
}
