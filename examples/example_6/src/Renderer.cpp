#include <string>
#include <iostream>
#include "Renderer.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "shader.h"

void Renderer::initalize() {
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, MAX_TRIANGLES * sizeof(Triangle), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void *) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void *) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    std::cout << offsetof(Vertex, color) << std::endl;

    projection = glm::ortho(-2.0, 2.0, -2.0, 2.0, -0.01, 1.0);

    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 430 core

        layout(location = 0) in vec2 position;
        layout(location = 1) in vec4 vertex_color;

        out vec4 out_color;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(position.xy, 0.0, 1.0);
            out_color = vertex_color;
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 430 core

        in vec4 vertex_color;

        out vec4 fragment_color;

        void main() {
            fragment_color = vec4(vertex_color);
        }
    )";

    shaderId = createShader(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderId);

    auto projectionLocation = glGetUniformLocation(shaderId, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void Renderer::free() const {
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &vertexArrayId);

    glDeleteProgram(shaderId);
}

void Renderer::beginFrame() {
    triangles.clear();
}

void Renderer::endFrame() {
    glUseProgram(shaderId);
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, triangles.size() * sizeof(Triangle), triangles.data());

    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
}

void Renderer::pushTriangle(Triangle triangle) {
    if (triangles.size() == MAX_TRIANGLES) {
        std::cerr << "Max triangles reached" << std::endl;
        return;
    }

    triangles.push_back(triangle);
}
