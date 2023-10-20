#include "ChessPieces.h"
#include "framework/geometry.h"
#include "glm/gtx/euler_angles.hpp"
#include "constants.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;

    out VertexData {
        vec3 position;
        vec3 texture_coordinates;
        vec4 color;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    struct InstanceData {
        ivec2 piece_positions;
        vec4 color;
    };

    layout(std140) uniform InstanceBuffer {
        InstanceData instances[8 * 4];
    };

    void main() {
        InstanceData instance_data = instances[gl_InstanceID];

        vertex_data.position = (model * vec4(position, 1.0)).xyz;
        vertex_data.texture_coordinates = position;
        vertex_data.color = instance_data.color;

        ivec2 piece_position = instance_data.piece_positions;
        // TODO magic number
        float offset = 100. / 32.;
        vec2 piece_offset = vec2(piece_position.x * offset, -piece_position.y * offset);
        gl_Position = projection * view * model * vec4(position.xy + piece_offset, position.z, 1.0);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec3 position;
        vec3 texture_coordinates;
        vec4 color;
    } vertex_data;

    out vec4 color;

    layout(binding=0) uniform samplerCube texture_sampler;
    uniform bool use_textures;

    void main() {
        vec4 texture_color = texture(texture_sampler, vertex_data.texture_coordinates);

        color = mix(vertex_data.color, texture_color, use_textures ? 0.5 : 0);
    }
)";

static glm::mat4 modelMatrix() {
    auto modelMatrix = glm::mat4(1.0f);

    // TODO magic number
    float unit = 8.f / 32.f;
    // Transform model to {0, 0} on the board
    modelMatrix = glm::translate(modelMatrix, glm::vec3(unit * -3.5f, unit * 3.5f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08f));

    // Float above board
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 1.2f));

    return modelMatrix;
}

ChessPieces ChessPieces::create(const std::vector<InstanceData> &pieces) {
    auto cubeShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);
    cubeShader->uploadUniformMatrix4("model", modelMatrix());

    // Chessboard mesh
    auto cubeVertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return ChessPieces::Vertex{
                .position = position,
            };
        });

    auto cubeIndices = framework::unitCube::indices;

    // VertexArray
    auto object = framework::VertexArray<ChessPieces::Vertex>::create(
        cubeShader,
        {
            {.type =GL_FLOAT, .size = 3, .offset = offsetof(ChessPieces::Vertex, position)},
        },
        {cubeVertices.begin(), cubeVertices.end()},
        cubeIndices
    );

    auto texture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/cube_texture.png"));

    auto instanceBuffer = framework::UniformBuffer<InstanceData>::create(pieces);

    return {
        .object = std::move(object),
        .texture = std::move(texture),
        .instanceBuffer = std::move(instanceBuffer)
    };
}

void ChessPieces::updatePieces(const std::vector<InstanceData> &pieces) const {
    instanceBuffer.updateData(pieces);
}

void ChessPieces::draw(bool useTextures, framework::Camera camera) const {
    object.shader->uploadUniformBuffer("InstanceBuffer", 0, instanceBuffer);
    object.shader->uploadUniformBool1("use_textures", useTextures);
    object.shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    object.shader->uploadUniformMatrix4("view", camera.viewMatrix());

    texture.bind();
    object.drawInstanced(BOARD_PIECES);
}