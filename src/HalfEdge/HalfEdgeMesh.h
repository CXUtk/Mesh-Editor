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

    private:
        // Create new elements
        PFace newFace();
        PHalfEdge newHalfEdge(PVertex a, PVertex b);
        PEdge newEdge(PHalfEdge a, PHalfEdge b);
        PVertex newVertex(const glm::vec3& pos);

        // Construct the properties for a face by using 3 half edges
        void constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3);
        void connectEdge(PHalfEdge A, PHalfEdge B);

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

        std::shared_ptr<AccelStructure> _accelStructure;
    };
}
