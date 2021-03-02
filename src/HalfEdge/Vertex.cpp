#include "Vertex.h"
#include <glm/gtx/transform.hpp>
#include <HalfEdge/HalfEdge.h>
#include <HalfEdge/Edge.h>
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

    glm::vec3 DCEL::Vertex::GetNormal() const {
        return glm::vec3();
    }

    void DCEL::Vertex::DrawOnScene(std::shared_ptr<Renderer> renderer, glm::vec4 color) const {
        // renderer->DrawCircle();
    }

    std::vector<PEdge> DCEL::Vertex::GetAdjEdges() const {
        std::vector<PEdge> edges;
        PHalfEdge hedge = _halfEdge;
        do {
            edges.push_back(hedge->Edge());
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return edges;
    }

    std::vector<PHalfEdge> DCEL::Vertex::GetAdjHalfEdges() const {
        std::vector<PHalfEdge> edges;
        PHalfEdge hedge = _halfEdge;
        do {
            edges.push_back(hedge);
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return edges;
    }
    std::vector<PVertex> Vertex::GetAdjVertices() const {
        std::vector<PVertex> vertices;
        PHalfEdge hedge = _halfEdge;
        do {
            vertices.push_back(hedge->To());
            hedge = hedge->Twin()->Next();
        } while (hedge != _halfEdge);
        return vertices;
    }
}