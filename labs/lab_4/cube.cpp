#include "cube.h"
#include "framework/Camera.h"

void Cube::draw() const {
    texture.bind();

    // Set model matrix
    glm::dvec2 cursorPosition;
    glfwGetCursorPos(window, &cursorPosition.x, &cursorPosition.y);

    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.2f, 0.f));
    modelMatrix *= glm::eulerAngleXY((float) cursorPosition.y / 500.f, (float) cursorPosition.x / 500.f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

    object.shader->uploadUniformMatrix4("model", modelMatrix);

    // Draw fill
    object.shader->uploadUniformFloat4("color", {1.f, 1.f, 1.f, 0.5f});
    object.draw();

    // Draw wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    object.shader->uploadUniformFloat4("color", {0.f, 0.f, 0.f, 1.f});
    object.draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Cube createCube(GLFWwindow *window, framework::Camera camera) {
    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 450 core

        layout(location = 0) in vec3 position;

        out VertexData {
            vec3 position;
        } vertex_data;

        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;

        void main() {
            vertex_data.position = position;
            gl_Position = projection * view * model * vec4(position.xyz, 1.0);
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 450 core

        in VertexData {
            vec3 position;
        } vertex_data;

        out vec4 fragment_color;

        layout(binding=0) uniform samplerCube texture_sampler;
        uniform vec4 color;

        void main() {
            fragment_color = texture(texture_sampler, vertex_data.position) * color;
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
    cubeShader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    cubeShader->uploadUniformMatrix4("view", camera.viewMatrix);

    auto object = framework::VertexArrayObject<Cube::Vertex>::create(
        cubeShader,
        {
            {.type =GL_FLOAT, .size = 3, .offset = offsetof(Cube::Vertex, position)},
        },
        {cubeVertices.begin(), cubeVertices.end()},
        cubeIndices
    );

    auto texture = framework::loadCubemap(TEXTURES_DIR + std::string("concrete.png"));

    return {
        .window = window,
        .object = std::move(object),
        .texture = std::move(texture)
    };
}