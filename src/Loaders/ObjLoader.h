#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <memory>

#include <glm/glm.hpp>

#include "ObjTriangle.h"
#include "Structures/HalfEdgeMesh.h"
#include "Test/Mesh.h"



class ObjLoader {
public:

    void load(const std::string& path);
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<ObjTriangle> Triangles;

    Mesh getMesh() const;
    std::vector<DrawTriangle> GetDrawTriangles() const;
private:
    static constexpr int MAX_BUFFER = 100005;
    static char lineBuffer[MAX_BUFFER];
    int _ptr;
    int _totV;


    void process();
    char getNext() { return lineBuffer[++_ptr]; }
    char getCur() { return lineBuffer[_ptr]; }
    char getPrev() { return lineBuffer[--_ptr]; }
    void delSpace() {
        char c = lineBuffer[_ptr];
        while (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            c = getNext();
        }
    }
};
