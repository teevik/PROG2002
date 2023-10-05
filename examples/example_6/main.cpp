#include <string>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "framework/window.h"
#include "framework/geometry.h"

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 1");

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

        float random(vec2 st) {
            return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
        }

        out vec4 color;

        void main() {
            color = vec4(random(vec2(gl_PrimitiveID / 2, 0)), random(vec2(gl_PrimitiveID / 2, 1)), random(vec2(gl_PrimitiveID / 2, 2)), 1);
        })";

    auto grid = framework::generateGridMesh(8);

    auto shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    auto object = framework::createVertexArrayObject<glm::vec2>(
        shader,
        {
            {.type =GL_FLOAT, .size = 2, .offset = 0},
        },
        grid.vertices,
        grid.indices
    );

    // Projection
    float aspectRatio = (float) width / (float) height;
    auto projection = glm::ortho(-0.8f * aspectRatio, 0.8f * aspectRatio, -0.8f, 0.8f, -0.01f, 1.0f);

    shader->uploadUniformMatrix4("projection", projection);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        object.draw(GL_TRIANGLES);

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