#include "TestMesh.h"

TestMeshes::TestMeshes() {
    Mesh mesh;

    glm::vec3 points[10] = {
        glm::vec3(-1, 0, 0),    // 1
        glm::vec3(0, 1, 0),     // 2
        glm::vec3(1, 0, 0),     // 3
        glm::vec3(0, -1, 0),    // 4
        glm::vec3(1, 1, 0),     // 5
        glm::vec3(-1, 1, 0),    // 6
        glm::vec3(-1, -1, 0),   // 7
        glm::vec3(1, -1, 0),    // 8
    };

    std::vector<ObjTriangle> triangles;
    triangles.push_back(ObjTriangle(1, 3, 2));
    triangles.push_back(ObjTriangle(1, 2, 6));
    triangles.push_back(ObjTriangle(3, 4, 8));
    triangles.push_back(ObjTriangle(3, 5, 2));


    for (int i = 0; i < 8; i++) {
        mesh.V.push_back(points[i]);
    }
    mesh.F = triangles;
    _meshes.push_back(mesh);



}

TestMeshes::~TestMeshes() {
}
