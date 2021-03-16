#include "DCEL.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Edge.h"
#include "Vertex.h"
#include <Structures/VertexData.h>
#include <Core/Loaders/TriangleMesh.h>
#include <Core/Graphics/RenderUtils.h>
#include <Structures/AccelStructure/AccelStructure.h>

#include <memory>
#include <list>
#include <vector>


namespace DCEL {

    struct Node {
        PEdge edge;
        glm::vec3 optimalPos;
        float cost;
        Node() = default;
        Node(PEdge e) :edge(e) {
            glm::mat4 quadratic = e->HalfEdge()->From()->GetQuadratic();
            quadratic += e->HalfEdge()->To()->GetQuadratic();

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

    /// <summary>
    /// Half edge data structure, assume the edges of all the faces are ordered counter-clockwise
    /// </summary>
    class HalfEdgeMesh {
    public:
        HalfEdgeMesh();
        ~HalfEdgeMesh();

        /// <summary>
        /// Build the half edge mesh with given vertices and indices
        /// Indices should be 0-indexed
        /// </summary>
        /// <param name="vertices"></param>
        /// <param name="faces"></param>
        void Build(const TriangleMesh& triangleMesh);


        std::vector<DrawTriangle> GetDrawTriangles() const;
        std::vector<DrawSegment> GetDrawWireFrames() const;

        bool RayIntersect(const Ray& ray, HitRecord& hit) const;

        void Recalculate();

        // Edge Operations 
        void FlipEdge(const_PEdge edge);
        PVertex SplitEdge(const_PEdge edge, EdgeOpPositionFunction posFunc);
        PVertex CollapseEdge(PEdge edge, EdgeOpPositionFunction posFunc);


        // Global Operations
        void LoopSubdivision();
        void DownSample();

        void ConstructQuadraticQueue();
        const_PEdge NextCollapseEdge() const { return _nextCollapseEdge; }

    private:
        // Create new elements
        PFace newFace();
        PHalfEdge newHalfEdge(PVertex a, PVertex b);
        PEdge newEdge(PHalfEdge a, PHalfEdge b);
        PVertex newVertex(const glm::vec3& pos);

        // Construct the properties for a face by using 3 half edges
        PFace constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3);
        PEdge connectEdge(PHalfEdge A, PHalfEdge B);

        void removeFace(PFace face);
        void removeEdge(PEdge edge);
        void removeHalfEdge(PHalfEdge halfedge);
        void removeVertex(PVertex vertex);

        void rebuildAccelStructure();
        void do_remove();


        std::list<Face> _faces;
        std::list<HalfEdge> _halfEdges;
        std::list<Edge> _edges;
        std::list<Vertex> _vertices;

        int _totF, _totHE, _totE, _totV;
        PEdge _nextCollapseEdge;

        std::shared_ptr<AccelStructure> _accelStructure;
    };
}
