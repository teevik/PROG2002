#include <string>
#include <vector>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "framework/window.h"
#include "framework/geometry.h"
#include "framework/Camera.h"

const int BOARD_SIZE = 8;

struct Vertex {
    /// Vertex position
    glm::vec2 position;

    /// Position between {0.f, 0.f} (top left corner) and {1.f, 1.f} (bottom right corner)
    glm::vec2 gridPosition;
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

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 2");

    // Chessboard mesh
    std::vector<Vertex> chessboardVertices = {
        { // right top
            .position = {1.f, 1.f},
            .gridPosition = {1.f, 0.f}
        },
        { // right bottom
            .position = {1.f, -1.f},
            .gridPosition = {1.f, 1.f}
        },
        { // left top
            .position = {-1.f, 1.f},
            .gridPosition = {0.f, 0.f}
        },
        { // left bottom
            .position = {-1.f, -1.f},
            .gridPosition = {0.f, 1.f}
        },
    };

    std::vector<uint32_t> chessboardIndices = {
        0, // right top
        1, // right bottom
        2, // left top
        1, // right bottom
        3, // left bottom
        2 // left top
    };

    auto chessboardShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    auto object = framework::VertexArray<Vertex>(
        chessboardShader,
        {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Vertex, gridPosition)},
        },
        framework::VertexBuffer(chessboardVertices),
        framework::IndexBuffer(chessboardIndices)
    );

    // Projection
    float aspectRatio = (float) width / (float) height;
    auto camera = framework::Camera::createOrthographic(1.5f, aspectRatio, {}, {}, {});

    chessboardShader->uploadUniformMatrix4("projection", camera.projectionMatrix);

    // Board size
    chessboardShader->uploadUniformInt1("board_size", BOARD_SIZE);

    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // State of selected tile
    static glm::ivec2 selectedTile = {0, 0};

    auto keyCallback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_LEFT:
                    if (selectedTile.x > 0) selectedTile.x -= 1;
                    break;

                case GLFW_KEY_RIGHT:
                    if (selectedTile.x < BOARD_SIZE - 1) selectedTile.x += 1;
                    break;

                case GLFW_KEY_UP:
                    if (selectedTile.y > 0) selectedTile.y -= 1;
                    break;

                case GLFW_KEY_DOWN:
                    if (selectedTile.y < BOARD_SIZE - 1) selectedTile.y += 1;
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

    glfwTerminate();

    return EXIT_SUCCESS;
}