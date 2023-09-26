#include <string>
#include <vector>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "framework/window.h"
#include "framework/geometry.h"

const int BOARD_SIZE = 8;

struct Vertex {
    glm::vec2 position;
    glm::vec2 gridPosition;
};

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 2");

    // Chessboard mesh
    std::vector<framework::Triangle<Vertex>> chesboardTriangles = {
        {
            .a = { // right top
                .position = {1.f, 1.f},
                .gridPosition = {1.f, 0.f}
            },
            .b = { // right bottom
                .position = {1.f, -1.f},
                .gridPosition = {1.f, 1.f}
            },
            .c = { // left top
                .position = {-1.f, 1.f},
                .gridPosition = {0.f, 0.f}
            }
        },
        {
            .a = { // right bottom
                .position = {1.f, -1.f},
                .gridPosition = {1.f, 1.f}
            },
            .b = { // left bottom
                .position = {-1.f, -1.f},
                .gridPosition = {0.f, 1.f}
            },
            .c = { // left top
                .position = {-1.f, 1.f},
                .gridPosition = {0.f, 0.f}
            }
        }
    };

    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 450 core

        layout(location = 0) in vec2 position;
        layout(location = 1) in vec2 grid_position;

        out VertexData {
            vec2 grid_position;
        } vertex_data;

        uniform mat4 projection;

        void main() {
            vertex_data.grid_position = grid_position;
            gl_Position = projection * vec4(position.xy, 0.0, 1.0);
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 450 core

        in VertexData {
            vec2 grid_position;
        } vertex_data;

        out vec4 color;

        uniform int board_size;
        uniform ivec2 selected_tile;

        const vec4 white = vec4(1, 1, 1, 1);
        const vec4 black = vec4(0, 0, 0, 1);
        const vec4 green = vec4(0, 1, 0, 1);

        void main() {
            ivec2 tile_index = ivec2(floor(vertex_data.grid_position * board_size));

            bool is_black = tile_index.x % 2 == tile_index.y % 2;
            bool is_selected = tile_index == selected_tile;

            color = is_selected ? green : (
                is_black ? black : white
            );
        }
    )";

    auto chessboardShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    auto object = framework::VertexArrayObjectBuilder<Vertex>{
        .shader = chessboardShader,
        .attributes = {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, gridPosition)},
        },
        .triangles = chesboardTriangles
    }.build();

    // Projection
    float aspectRatio = (float) width / (float) height;
    float scale = 1.5f;
    auto projection = glm::ortho(-scale * aspectRatio, scale * aspectRatio, -scale, scale, -0.01f, 1.0f);
    chessboardShader->uploadUniformMatrix4("projection", projection);

    // Board size
    chessboardShader->uploadUniformInt1("board_size", BOARD_SIZE);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // State of selected tile
    glm::ivec2 selectedTile = {0, 0};

    // Store selected tile in glfw user pointer to access in callback, will probably store the whole game state in
    // the future?
    glfwSetWindowUserPointer(window, &selectedTile);
    auto keyCallback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto selectedTile = static_cast<glm::ivec2 *>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_LEFT:
                    if (selectedTile->x > 0) selectedTile->x -= 1;
                    break;

                case GLFW_KEY_RIGHT:
                    if (selectedTile->x < BOARD_SIZE - 1) selectedTile->x += 1;
                    break;

                case GLFW_KEY_UP:
                    if (selectedTile->y > 0) selectedTile->y -= 1;
                    break;

                case GLFW_KEY_DOWN:
                    if (selectedTile->y < BOARD_SIZE - 1) selectedTile->y += 1;
                    break;

                default:
                    break;
            }
        }
    };

    glfwSetKeyCallback(window, keyCallback);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set selected_tile uniform
        chessboardShader->uploadUniformInt2("selected_tile", selectedTile);

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