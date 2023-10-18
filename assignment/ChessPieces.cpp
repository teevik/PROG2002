#include "ChessPieces.h"
#include "framework/geometry.h"
#include "glm/gtx/euler_angles.hpp"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;

    out VertexData {
        vec3 position;
        vec3 texture_coordinates;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        vertex_data.position = (model * vec4(position, 1.0)).xyz;
        vertex_data.texture_coordinates = position;

        gl_Position = projection * view * model * vec4(position.xyz, 1.0);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec3 position;
        vec3 texture_coordinates;
    } vertex_data;

    out vec4 fragment_color;

    layout(binding=0) uniform samplerCube texture_sampler;
//    uniform bool use_textures;

    void main() {
        fragment_color = texture(texture_sampler, vertex_data.texture_coordinates);
    }
)";

ChessPieces ChessPieces::create() {
    auto cubeShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    // Chessboard mesh
    auto cubeVertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return ChessPieces::Vertex{
                .position = position,
            };
        });

    auto cubeIndices = framework::unitCube::indices;

    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.2f, 0.f));
    modelMatrix *= glm::eulerAngleXY(0.5f, 0.5f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

    cubeShader->uploadUniformMatrix4("model", modelMatrix);

    auto object = framework::VertexArrayObject<ChessPieces::Vertex>::create(
        cubeShader,
        {
            {.type =GL_FLOAT, .size = 3, .offset = offsetof(ChessPieces::Vertex, position)},
        },
        {cubeVertices.begin(), cubeVertices.end()},
        cubeIndices
    );

    auto texture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/cube_texture.png"));

    return {
        .object = std::move(object),
        .texture = std::move(texture)
    };
}

void ChessPieces::draw(bool useTextures, framework::Camera camera) const {
    object.shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    object.shader->uploadUniformMatrix4("view", camera.viewMatrix);

    texture.bind();
    object.draw();
}
