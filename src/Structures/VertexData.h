#pragma once
#include <glm/glm.hpp>
#include <tuple>
#include <vector>
struct VertexData {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;

    VertexData() {}
    VertexData(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texCoord)
        : Position(pos), Normal(normal), TexCoord(texCoord) {
    }
};


struct TriangleFaceIndex {
    int VertexID[3], NormalID[3], TexID[3];
    TriangleFaceIndex() {}
    TriangleFaceIndex(const std::vector<std::tuple<int, int, int>>& vs) {
        int i = 0;
        for (auto [a, b, c] : vs) {
            VertexID[i] = a;
            NormalID[i] = b;
            TexID[i] = c;
            i++;
        }
    }
    TriangleFaceIndex(int a, int b, int c) {
        VertexID[0] = a;
        VertexID[1] = b;
        VertexID[2] = c;
    }
};
