#include <string>
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "shader.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "Mesh.h"

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

int main() {
    int width = 800;
    int height = 600;

    auto window = createWindow(width, height);

    // TODO: new structure
    // DrawPass
    //     holds one shader pipeline
    //     holds one list of VertexAttribute
    //     holds multiple static meshes
    //     ?holds multiple dynamic meshes (future)

    auto mesh = Mesh<Vertex>{
        .attributes = {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 4, .offset = offsetof(Vertex, color)}
        }
    };
    mesh.pushTriangles(
        {
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
    );

    auto gpuMesh = mesh.initialize();

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

    // Shader
    uint32_t shaderId = createShaderPipeline(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderId);

    // Projection
    float aspectRatio = (float) width / (float) height;
    auto projection = glm::ortho(-2.0f * aspectRatio, 2.0f * aspectRatio, -2.0f, 2.0f, -0.01f, 1.0f);
    auto projectionLocation = glGetUniformLocation(shaderId, "projection");
    assert(projectionLocation != -1);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set time uniform
        auto time = (float) glfwGetTime();
        int32_t timeLocation = glGetUniformLocation(shaderId, "time");
        assert(timeLocation != -1);
        glUniform1f(timeLocation, time);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        gpuMesh.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    gpuMesh.free();
    glDeleteProgram(shaderId);

    glfwTerminate();

    return EXIT_SUCCESS;
}