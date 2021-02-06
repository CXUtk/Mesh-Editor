#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include "Loaders/ObjTriangle.h"
#include "Graphics/RenderUtils.h"
#include "Ray.h"
#include <string>
#include <list>
#include "DCEL.h"
#include "Test/Mesh.h"
#include "AccelStructure/AccelStructure.h"


namespace DCEL {
    class HalfEdgeMesh {
    public:
        HalfEdgeMesh();
        HalfEdgeMesh(const std::vector<glm::vec3>& V, const std::vector<ObjTriangle>& F);
        ~HalfEdgeMesh();

        void build(const Mesh& mesh);

        std::vector<Segment> getEdges() const;
        std::vector<Segment> getEdgesMarked() const;
        std::vector<DrawTriangle> getTriangles() const;
        /*   std::vector<DrawTriangle> getTrianglesMarked() const;*/
        void save(const std::string& filename) const;
        DrawTriangle getDrawFace(const_PFace face) const;
        Segment getDrawEdge(const_PEdge edge) const;


        bool testHitTriangle(const Ray& ray, IntersectionInfo& triangle) const;

        void flipEdge(PHalfEdge e);
        PVertex splitEdge(PHalfEdge e, bool loop);
        PVertex collapseEdge(PHalfEdge e, glm::vec3 pos);

        void upSample();
        void downSample(int maxFaces);
        void reSample();
        void bulkCheck();


        int getFaceCount()const { return _faces.size(); }
        int getVertexCount()const { return _vertices.size(); }
        int getEdgeCount()const { return _edges.size(); }


    private:
        PVertex newVertex(const glm::vec3& pos);
        PFace newFace();
        PHalfEdge newHalfEdge(PVertex fr, PVertex to);
        PEdge newEdge(PHalfEdge a, PHalfEdge b);
        PEdge newEdgeBroken(PHalfEdge a);


        void constructFace(PFace face, PHalfEdge e1, PHalfEdge e2, PHalfEdge e3);



        void deleteVertex(PVertex v);
        void deleteHalfEdge(PHalfEdge e);
        void deleteEdge(PEdge e);
        void deleteFace(PFace f);


        void makeTwin(PHalfEdge A, PHalfEdge B);
        void updateAccelStructure();
        void removeFromList();


        std::list<Vertex> _vertices;
        std::list<HalfEdge> _halfEdges;
        std::list<Face> _faces;
        std::list<Edge> _edges;

        std::shared_ptr<AccelStructure> _accelStructure;

        int _totV, _totF, _totE, _totHE;

    };
}
