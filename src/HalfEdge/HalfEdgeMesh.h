#include "DCEL.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Edge.h"
#include "Vertex.h"
#include <Structures/VertexData.h>
#include <Core/Loaders/TriangleMesh.h>
#include <Core/Graphics/RenderUtils.h>

#include <memory>
#include <list>
#include <vector>


namespace DCEL {
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
        std::vector<Segment> GetDrawWireFrames() const;

    private:
        // Create new elements
        PFace newFace();
        PHalfEdge newHalfEdge(PVertex a, PVertex b);
        PEdge newEdge(PHalfEdge a, PHalfEdge b);
        PVertex newVertex(const glm::vec3& pos);

        // Construct the properties for a face by using 3 half edges
        void constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3);


        std::list<Face> _faces;
        std::list<HalfEdge> _halfEdges;
        std::list<Edge> _edges;
        std::list<Vertex> _vertices;

        int _totF, _totHE, _totE, _totV;
    };
}
