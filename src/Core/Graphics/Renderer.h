#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "RenderUtils.h"
#include "Shaders/ShaderManager.h"
#include <algorithm>

class Renderer {
public:
    Renderer();
    ~Renderer();
    void drawLines(const std::vector<DrawSegment>& lines, const glm::vec3& color, float width);
    void drawLightedTriangles(const std::vector<DrawTriangle>& triangles, const glm::vec3& color, const glm::vec3& lightDir, const glm::vec3& eyePos);
    void drawTriangles(const std::vector<DrawTriangle>& triangles, const glm::vec4& color);
    void begin(glm::mat4 projection, glm::mat4 view);
    void end();
private:

    std::shared_ptr<ShaderManager> _shaderManager;
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;

    void init_lineBuffer();
    void init_triangleBuffer();
};
