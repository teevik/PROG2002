#include "ChessPieces.h"
#include "framework/geometry.h"
#include "constants.h"
#include <regex>

std::string vertexShaderSource() {
    // language=glsl
    std::string shader = R"(
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

        uniform ivec2 selected_tile;
        uniform ivec2 piece_being_moved;

        struct InstanceData {
            ivec2 position;
            vec4 color;
        };

        layout(std140) uniform InstanceBuffer {
            InstanceData instances[BOARD_PIECES];
        };

        const vec4 GREEN = vec4(0, 1, 0, 1);
        const vec4 YELLOW = vec4(1, 1, 0, 1);

        void main() {
            InstanceData instance_data = instances[gl_InstanceID];

            vertex_data.position = (model * vec4(position, 1.0)).xyz;
            vertex_data.texture_coordinates = position;

            ivec2 piece_position = instance_data.position;

            if (piece_position == piece_being_moved) {
                vertex_data.color = YELLOW;
            } else if (piece_position == selected_tile) {
                vertex_data.color = GREEN;
            } else {
                vertex_data.color = instance_data.color;
            }

            float offset = 4. / (float(BOARD_SIZE));

            // Position of {0, 0} on the board
            vec2 piece_origin = vec2(-2 + offset / 2., 2 - offset / 2.);

            // Offset from {0, 0}
            vec2 piece_offset = vec2(offset, -offset) * piece_position;

            gl_Position =
                projection * view * model * vec4(position.xyz, 1.0) + // Mesh position
                projection * view * vec4(piece_origin + piece_offset, 0, 1); // Instance position
        }
    )";

    // Replace constants with actual value
    shader = std::regex_replace(shader, std::regex("BOARD_SIZE"), std::to_string(BOARD_SIZE));
    shader = std::regex_replace(shader, std::regex("BOARD_PIECES"), std::to_string(BOARD_PIECES));

    return shader;
}


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

    modelMatrix = glm::scale(modelMatrix, glm::vec3(PIECE_SCALE));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 1.2f));

    return modelMatrix;
}

ChessPieces ChessPieces::create(const std::vector<InstanceData> &pieces) {
    auto cubeShader = std::make_shared<framework::Shader>(vertexShaderSource(), fragmentShaderSource);
    cubeShader->uploadUniformMatrix4("model", modelMatrix());

    auto cubeVertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return ChessPieces::Vertex{
                .position = position,
            };
        });

    auto cubeIndices = framework::unitCube::indices;

    auto vertexArray = framework::VertexArray(
        cubeShader,
        {
            {.type = GL_FLOAT, .size = 3, .offset = offsetof(ChessPieces::Vertex, position)},
        },
        framework::VertexBuffer<ChessPieces::Vertex>({cubeVertices.begin(), cubeVertices.end()}),
        framework::IndexBuffer(cubeIndices)
    );

    auto texture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/cube_texture.png"));

    auto instanceBuffer = framework::UniformBuffer<InstanceData>::create(pieces);

    return {
        .vertexArray = std::move(vertexArray),
        .texture = std::move(texture),
        .instanceBuffer = std::move(instanceBuffer)
    };
}

void ChessPieces::updatePieces(const std::vector<InstanceData> &pieces) const {
    instanceBuffer.updateData(pieces);
}

void ChessPieces::draw(
    glm::ivec2 selectedTile,
    std::optional<glm::ivec2> pieceBeingMoved,
    bool useTextures,
    const framework::Camera &camera
) const {
    vertexArray.shader->uploadUniformInt2("selected_tile", selectedTile);
    vertexArray.shader->uploadUniformInt2("piece_being_moved", pieceBeingMoved.value_or(glm::ivec2(-1, -1)));

    vertexArray.shader->uploadUniformBuffer("InstanceBuffer", 0, instanceBuffer);
    vertexArray.shader->uploadUniformBool1("use_textures", useTextures);
    vertexArray.shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    vertexArray.shader->uploadUniformMatrix4("view", camera.viewMatrix());

    texture.bind();
    vertexArray.drawInstanced(BOARD_PIECES);
}