#include <string>
#include <vector>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "framework/window.h"
#include "framework/geometry.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

const int BOARD_SIZE = 8;

struct Chessboard {
    struct Vertex {
        /// Vertex position
        glm::vec2 position;

        /// Position between {0.f, 0.f} (top left corner) and {1.f, 1.f} (bottom right corner)
        glm::vec2 gridPosition;
    };

    const framework::VertexArrayObject<Vertex> object;
    glm::ivec2 selectedTile;

    static Chessboard create(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
        // language=glsl
        const std::string vertexShaderSource = R"(
            #version 450 core

            layout(location = 0) in vec2 position;
            layout(location = 1) in vec2 grid_position;

            out VertexData {
                vec2 grid_position;
            } vertex_data;

            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;

            void main() {
                vertex_data.grid_position = grid_position;
                gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
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

        // Chessboard mesh
        std::vector<Chessboard::Vertex> chessboardVertices = {
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

        auto chessboardModelMatrix = glm::mat4(1.0f);
        chessboardModelMatrix = glm::scale(chessboardModelMatrix, glm::vec3(5));
        chessboardModelMatrix = glm::rotate(chessboardModelMatrix, glm::radians(-80.f), glm::vec3(1.0, 0.0, 0.0));
        chessboardModelMatrix = glm::translate(chessboardModelMatrix, glm::vec3(0.f, 1.f, -0.5f));

        // Transformation
        chessboardShader->uploadUniformMatrix4("projection", projectionMatrix);
        chessboardShader->uploadUniformMatrix4("view", viewMatrix);
        chessboardShader->uploadUniformMatrix4("model", chessboardModelMatrix);

        // Board size
        chessboardShader->uploadUniformInt1("board_size", BOARD_SIZE);

        auto object = framework::VertexArrayObject<Chessboard::Vertex>::create(
            chessboardShader,
            {
                {.type =GL_FLOAT, .size = 2, .offset = offsetof(Chessboard::Vertex, position)},
                {.type =GL_FLOAT, .size = 2, .offset = offsetof(Chessboard::Vertex, gridPosition)},
            },
            chessboardVertices,
            chessboardIndices
        );

        return {
            .object = std::move(object),
            .selectedTile = {0, 0}
        };
    }

    void draw() const {
        object.shader->uploadUniformInt2("selected_tile", selectedTile);
        object.draw();
    }

    void handleKeyInput(int key, int action) {
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
    }
};

struct Cube {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;
    };

    GLFWwindow *window;
    const framework::VertexArrayObject<Vertex> object;

    static Cube create(GLFWwindow *window, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
        // language=glsl
        const std::string vertexShaderSource = R"(
            #version 450 core

            layout(location = 0) in vec3 position;

            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;

            void main() {
                gl_Position = projection * view * model * vec4(position.xyz, 1.0);
            }
        )";

        // language=glsl
        const std::string fragmentShaderSource = R"(
            #version 450 core
            out vec4 fragment_color;

            uniform vec4 color;

            void main() {
                fragment_color = color;
            }
        )";

        auto cubeShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

        // Chessboard mesh
        auto cubeVertices =
            framework::unitCube::vertices | std::views::transform([](auto position) {
                return Cube::Vertex{
                    .position = position
                };
            });
        auto cubeIndices = framework::unitCube::indices;

        // Transformation, model is set in draw()
        cubeShader->uploadUniformMatrix4("projection", projectionMatrix);
        cubeShader->uploadUniformMatrix4("view", viewMatrix);

        auto object = framework::VertexArrayObject<Cube::Vertex>::create(
            cubeShader,
            {
                {.type =GL_FLOAT, .size = 3, .offset = offsetof(Cube::Vertex, position)},
            },
            {cubeVertices.begin(), cubeVertices.end()},
            cubeIndices
        );

        return {
            .window = window,
            .object = std::move(object),
        };
    }

    void draw() const {
        // Set model matrix
        glm::dvec2 cursorPosition;
        glfwGetCursorPos(window, &cursorPosition.x, &cursorPosition.y);

        auto modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.2f, 0.f));
        modelMatrix *= glm::eulerAngleXY((float) cursorPosition.y / 500.f, (float) cursorPosition.x / 500.f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

        object.shader->uploadUniformMatrix4("model", modelMatrix);

        // Draw fill
        object.shader->uploadUniformFloat4("color", {1.f, 1.f, 1.f, 1.f});
        object.draw();

        // Draw wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        object.shader->uploadUniformFloat4("color", {0.f, 0.f, 0.f, 1.f});
        object.draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 3");

    // Projection matrix
    float aspectRatio = (float) width / (float) height;
    auto projectionMatrix = glm::perspective(glm::radians(45.f), aspectRatio, 1.f, -10.f);

    // View matrix
    glm::vec3 position = {0.f, 0.f, 5.f};
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 1.f, 0.1f};

    auto viewMatrix = glm::lookAt(
        position,
        target,
        up
    );

    static auto chessboard = Chessboard::create(projectionMatrix, viewMatrix);
    auto cube = Cube::create(window, projectionMatrix, viewMatrix);

    // Handle input
    auto keyCallback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        chessboard.handleKeyInput(key, action);
    };

    glfwSetKeyCallback(window, keyCallback);

    glEnable(GL_DEPTH_TEST);
    // Clear color
    glClearColor(0.917f, 0.905f, 0.850f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chessboard.draw();
        cube.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}