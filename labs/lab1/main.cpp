#include <string>
#include <vector>
#include <memory>
#include <ranges>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "framework/window.h"
#include "framework/geometry.h"

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height);

    // Triangle
    auto triangle = framework::Triangle<Vertex>{
        .a = {
            .position = framework::unitTriangle2d.a,
            .color = {1.f, 1.f, 0.f, 1.f}
        },
        .b = {
            .position = framework::unitTriangle2d.b,
            .color = {0.f, 1.f, 1.f, 1.f}
        },
        .c = {
            .position = framework::unitTriangle2d.c,
            .color = {1.f, 0.f, 1.f, 1.f}
        }
    };
    framework::StaticMesh<Vertex> triangleMesh{.triangles = {triangle}};


    // Circle
    glm::vec2 circlePosition = {1.f, -1.f};
    auto circleColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    auto circleTriangles =
        framework::generateCircleMesh(32) | std::views::transform([circleColor, circlePosition](auto position) {
            return framework::Triangle<Vertex>{
                .a = {
                    .position = circlePosition + position.a,
                    .color = circleColor
                },
                .b = {
                    .position = circlePosition + position.b,
                    .color = circleColor
                },
                .c = {
                    .position = circlePosition + position.c,
                    .color = circleColor
                }
            };
        });

    framework::StaticMesh<Vertex> circleMesh{.triangles = {circleTriangles.begin(), circleTriangles.end()}};

    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 450 core

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
        #version 450 core

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

    std::shared_ptr<framework::Shader> shader(new framework::Shader(vertexShaderSource, fragmentShaderSource));

    auto object = framework::ObjectBuilder<Vertex>{
        .shader = shader,
        .attributes = {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 4, .offset = offsetof(Vertex, color)}
        },
        .staticMeshes = {
            triangleMesh,
            circleMesh
        },
    }.build();

    // Projection
    float aspectRatio = (float) width / (float) height;
    auto projection = glm::ortho(-2.0f * aspectRatio, 2.0f * aspectRatio, -2.0f, 2.0f, -0.01f, 1.0f);
    auto projectionLocation = glGetUniformLocation(object.shader->id, "projection");
    assert(projectionLocation != -1);
    glProgramUniformMatrix4fv(object.shader->id, projectionLocation, 1, false, &projection[0][0]);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set time uniform
        auto time = (float) glfwGetTime();
        int32_t timeLocation = glGetUniformLocation(object.shader->id, "time");
        assert(timeLocation != -1);
        glProgramUniform1f(object.shader->id, timeLocation, time);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        object.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    object.free();

    glfwTerminate();

    return EXIT_SUCCESS;
}