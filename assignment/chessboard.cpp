#include "chessboard.h"

#include "framework/VertexArrayObject.h"
#include "framework/Texture.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "framework/Camera.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 texture_coordinates;
    layout(location = 2) in vec2 grid_position;

    out VertexData {
        vec2 texture_coordinates;
        vec2 grid_position;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        vertex_data.texture_coordinates = texture_coordinates;
        vertex_data.grid_position = grid_position;
        gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec2 texture_coordinates;
        vec2 grid_position;
    } vertex_data;

    out vec4 color;

    layout(binding=0) uniform sampler2D texture_sampler;
    uniform int board_size;
    uniform ivec2 selected_tile;
    uniform float ambient_strength;

    const vec4 white = vec4(1, 1, 1, 1);
    const vec4 black = vec4(0, 0, 0, 1);
    const vec4 green = vec4(0, 1, 0, 1);

    void main() {
        ivec2 tile_index = ivec2(floor(vertex_data.grid_position * board_size));

        bool is_black = tile_index.x % 2 == tile_index.y % 2;
        bool is_selected = tile_index == selected_tile;

        vec4 chessboard_color = is_selected ? green : (
            is_black ? black : white
        );

        vec4 texture_color = texture(texture_sampler, vertex_data.texture_coordinates);

        color = vec4(vec3(ambient_strength), 1) * mix(chessboard_color, texture_color, 0.7);
    }
)";

Chessboard Chessboard::create(framework::Camera camera) {
    auto chessboardShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    // Chessboard mesh
    std::vector<Chessboard::Vertex> chessboardVertices = {
        { // right top
            .position = {1.f, 1.f},
            .textureCoordinates = {1.f, 0.f},
            .gridPosition = {1.f, 0.f}
        },
        { // right bottom
            .position = {1.f, -1.f},
            .textureCoordinates = {1.f, 1.f},
            .gridPosition = {1.f, 1.f}
        },
        { // left top
            .position = {-1.f, 1.f},
            .textureCoordinates = {0.f, 0.f},
            .gridPosition = {0.f, 0.f}
        },
        { // left bottom
            .position = {-1.f, -1.f},
            .textureCoordinates = {0.f, 1.f},
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
    chessboardShader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    chessboardShader->uploadUniformMatrix4("view", camera.viewMatrix);
    chessboardShader->uploadUniformMatrix4("model", chessboardModelMatrix);

    // Board size
    chessboardShader->uploadUniformInt1("board_size", BOARD_SIZE);

    auto object = framework::VertexArrayObject<Chessboard::Vertex>::create(
        chessboardShader,
        {
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Chessboard::Vertex, position)},
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Chessboard::Vertex, textureCoordinates)},
            {.type =GL_FLOAT, .size = 2, .offset = offsetof(Chessboard::Vertex, gridPosition)},
        },
        chessboardVertices,
        chessboardIndices
    );

    auto texture = framework::loadTexture(RESOURCES_DIR + std::string("textures/floor_texture.png"));

    return {
        .object = std::move(object),
        .texture = std::move(texture)
    };
}

void Chessboard::draw(glm::ivec2 selectedTile, float ambientStrength) const {
    object.shader->uploadUniformFloat1("ambient_strength", ambientStrength);
    object.shader->uploadUniformInt2("selected_tile", selectedTile);

    texture.bind();
    object.draw();
}