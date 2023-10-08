#include <string>
#include <vector>
#include <memory>
#include <ranges>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "framework/window.h"
#include "framework/geometry.h"
#include "framework/Camera.h"

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 1");

    // Triangle
    auto triangle = framework::unitTriangle | std::views::transform([](auto position) {
        return Vertex{
            .position = position,
            .color = {1.f, position.x, position.y, 1.f}
        };
    });

    // Circle
    glm::vec2 circlePosition = {1.f, -1.f};
    glm::vec4 circleColor = {1.f, 1.f, 1.f, 1.f};

    auto circleTriangles =
        framework::generateCircleMesh(32) | std::views::transform([circleColor, circlePosition](auto position) {
            return Vertex{
                .position = circlePosition + position,
                .color = circleColor
            };
        });

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

    auto shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    std::vector<Vertex> mesh;
    mesh.insert(mesh.end(), circleTriangles.begin(), circleTriangles.end());
    mesh.insert(mesh.end(), triangle.begin(), triangle.end());

    auto object = framework::VertexArrayObject<Vertex>::create(
        shader,
        {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 4, .offset = offsetof(Vertex, color)}
        },
        mesh,
        {}
    );

    // Camera
    float aspectRatio = (float) width / (float) height;
    auto camera = framework::Camera::createOrthographic(2.f, aspectRatio, {}, {}, {});

    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set time uniform
        auto time = (float) glfwGetTime();
        shader->uploadUniformFloat1("time", time);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        object.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}