#include <string>
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "DrawPass.h"

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

StaticMesh<Vertex> generateCircleMesh(glm::vec2 center, int resolution) {
    std::vector<Triangle<Vertex>> circleTriangles;

    for (int i = 0; i < resolution; i++) {
        float valueStart = (float) i / (float) resolution;
        float valueEnd = ((float) (i + 1) / (float) resolution);

        float angleStart = valueStart * glm::pi<float>() * 2.f;
        float angleEnd = valueEnd * glm::pi<float>() * 2.f;

        circleTriangles.push_back(
            {
                .a =   {
                    .position = center + glm::vec2(glm::cos(angleStart), glm::sin(angleStart)),
                    .color = {valueStart, valueStart, valueStart, 1.f}
                },
                .b =   {
                    .position = center + glm::vec2(glm::cos(angleEnd), glm::sin(angleEnd)),
                    .color = {valueEnd, valueEnd, valueEnd, 1.f}
                },
                .c =   {
                    .position = center,
                    .color = {1.f, 0.f, 1.f, 1.f}
                },
            }
        );
    }

    return StaticMesh<Vertex>{.triangles = std::move(circleTriangles)};
}

int main() {
    int width = 800;
    int height = 600;

    auto window = createWindow(width, height);

    auto twoTrianglesMesh = StaticMesh<Vertex>{
        .triangles = {
            {
                .a = {.position = glm::vec2(-0.9f, -1.f), .color = {1.f, 1.f, 0.f, 1.f}},
                .b = {.position = glm::vec2(1.f, -1.f), .color = {0.f, 1.f, 1.f, 1.f}},
                .c = {.position = glm::vec2(1.f, 0.9f), .color = {1.f, 0.f, 1.f, 1.f}}
            },
            {
                .a = {.position = glm::vec2(-1.f, -0.9f), .color = {1.f, 1.f, 0.f, 1.f}},
                .b = {.position = glm::vec2(-1.f, 1.f), .color = {0.f, 1.f, 1.f, 1.f}},
                .c = {.position = glm::vec2(0.9f, 1.f), .color = {1.f, 0.f, 1.f, 1.f}}
            }
        }
    };


    auto circleMesh = generateCircleMesh({1.f, -1.f}, 32);

    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 460 core

        layout(location = 0) in vec2 position;
        layout(location = 1) in vec4 color;

        out vec4 vertex_color;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(position.xy, 0.0, 1.0);
            vertex_color = color;
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 460 core

        float rand(float n){return fract(sin(n) * 43758.5453123);}

        float noise(float p){
            float fl = floor(p);
            float fc = fract(p);
            return mix(rand(fl), rand(fl + 1.0), fc);
        }

        in vec4 vertex_color;

        out vec4 color;

        uniform float time;

        void main() {
            color = vertex_color + vec4(cos(time), sin(time), cos(time), 1);
        }
    )";

    auto drawPass = DrawPass<Vertex>{
        .vertexShaderSource = vertexShaderSource,
        .fragmentShaderSource = fragmentShaderSource,
        .attributes = {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 4, .offset = offsetof(Vertex, color)}
        },
        .staticMeshes = {twoTrianglesMesh, circleMesh},
    }.initialize();

    // Projection
    float aspectRatio = (float) width / (float) height;
    auto projection = glm::ortho(-2.0f * aspectRatio, 2.0f * aspectRatio, -2.0f, 2.0f, -0.01f, 1.0f);
    auto projectionLocation = glGetUniformLocation(drawPass.shaderId, "projection");
    assert(projectionLocation != -1);
    glProgramUniformMatrix4fv(drawPass.shaderId, projectionLocation, 1, false, &projection[0][0]);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set time uniform
        auto time = (float) glfwGetTime();
        int32_t timeLocation = glGetUniformLocation(drawPass.shaderId, "time");
        assert(timeLocation != -1);
        glProgramUniform1f(drawPass.shaderId, timeLocation, time);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        drawPass.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    drawPass.free();

    glfwTerminate();

    return EXIT_SUCCESS;
}