#include "Renderer.h"
#include "Editor.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLuint _vaoLine, _vboLine;
static GLuint _vaoTriangle, _vboTriangle;
static constexpr int BUFFER_SIZE = 1024;
static constexpr int BUFFER_SIZE_T = 9999;

Renderer::Renderer() {
    _shaderManager = std::make_shared<ShaderManager>();

    //_viewMatrix = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    //_projectionMatrix = glm::perspective(glm::pi<float>() / 2.f, 800.0f / 600.0f, 1.f, 100.f);
    init_lineBuffer();
    init_triangleBuffer();
}

Renderer::~Renderer() {
}

void Renderer::drawLines(const std::vector<Segment>& lines, const glm::vec3& color, float width) {
    glLineWidth(width);
    auto model = glm::identity<glm::mat4>();
    auto shader = _shaderManager->getShaderData("line_draw");
    shader->apply();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, false, glm::value_ptr(_projectionMatrix * _viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, false, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->getID(), "uColor"), color.r, color.g, color.b);


    int sz = lines.size();
    glBindVertexArray(_vaoLine);
    // 以 BUFFER_SIZE 个点为单位，分批绘制线段
    for (int i = 0; i < sz; i += BUFFER_SIZE) {
        glBindBuffer(GL_ARRAY_BUFFER, _vboLine);
        int count = std::min(sz, i + BUFFER_SIZE) - i;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Segment) * count, lines.data() + i);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_LINES, 0, count * 2);
    }

    glBindVertexArray(0);
}

void Renderer::drawLightedTriangles(const std::vector<DrawTriangle>& triangles, const glm::vec3& color, const glm::vec3& lightDir, const glm::vec3& eyePos) {
    auto model = glm::identity<glm::mat4>();


    auto shader = _shaderManager->getShaderData("light_draw");
    shader->apply();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, false, glm::value_ptr(_projectionMatrix * _viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, false, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->getID(), "uColor"), color.r, color.g, color.b);
    glUniform3f(glGetUniformLocation(shader->getID(), "uLightDir"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(shader->getID(), "uEyePos"), eyePos.x, eyePos.y, eyePos.z);

    glUniform3f(glGetUniformLocation(shader->getID(), "ambientColor"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shader->getID(), "diffuseColor"), 0.7f, 0.7f, 0.7f);
    glUniform3f(glGetUniformLocation(shader->getID(), "specularColor"), 0.8f, 0.8f, 0.8f);


    int sz = triangles.size();
    glBindVertexArray(_vaoTriangle);
    // 以 BUFFER_SIZE_T 个顶点为单位，分批绘制三角形
    for (int i = 0; i < sz; i += BUFFER_SIZE_T / 3) {
        glBindBuffer(GL_ARRAY_BUFFER, _vboTriangle);
        int count = std::min(sz, i + BUFFER_SIZE_T / 3) - i;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DrawTriangle) * count, triangles.data() + i);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, count * 3);
    }

    glBindVertexArray(0);
}

void Renderer::drawTriangles(const std::vector<DrawTriangle>& triangles, const glm::vec3& color) {
    auto model = glm::identity<glm::mat4>();


    auto shader = _shaderManager->getShaderData("triangle_draw");
    shader->apply();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, false, glm::value_ptr(_projectionMatrix * _viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, false, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader->getID(), "uColor"), color.r, color.g, color.b);


    int sz = triangles.size();
    glBindVertexArray(_vaoTriangle);
    // 以 BUFFER_SIZE_T 个顶点为单位，分批绘制三角形
    for (int i = 0; i < sz; i += BUFFER_SIZE_T / 3) {
        glBindBuffer(GL_ARRAY_BUFFER, _vboTriangle);
        int count = std::min(sz, i + BUFFER_SIZE_T / 3) - i;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DrawTriangle) * count, triangles.data() + i);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, count * 3);
    }

    glBindVertexArray(0);
}

void Renderer::begin(glm::mat4 projection, glm::mat4 view) {
    _projectionMatrix = projection;
    _viewMatrix = view;
}

void Renderer::end() {
}

void Renderer::init_lineBuffer() {
    // 画线的顶点缓存
    glGenVertexArrays(1, &_vaoLine);
    glGenBuffers(1, &_vboLine);

    glBindVertexArray(_vaoLine);
    glBindBuffer(GL_ARRAY_BUFFER, _vboLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Segment) * BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::init_triangleBuffer() {
    // 画三角形面的顶点缓存
    glGenVertexArrays(1, &_vaoTriangle);
    glGenBuffers(1, &_vboTriangle);

    glBindVertexArray(_vaoTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, _vboTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * BUFFER_SIZE_T, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
