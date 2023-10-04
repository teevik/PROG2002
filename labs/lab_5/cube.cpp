#include "cube.h"
#include "framework/Camera.h"

void Cube::draw(float ambientStrength) const {
    object.shader->uploadUniformFloat1("ambient_strength", ambientStrength);

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
    object.shader->uploadUniformFloat4("color", {1.f, 1.f, 1.f, 1.f});
    object.draw();

    // Draw wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    object.shader->uploadUniformFloat4("color", {0.f, 0.f, 0.f, 1.f});
    object.draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cube::free() {
    object.free();
}

Cube createCube(GLFWwindow *window, framework::Camera camera) {
    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 450 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;

        out VertexData {
            vec3 position;
            vec3 texture_coordinates;
            vec3 normal;
        } vertex_data;

        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;

        void main() {
            vertex_data.position = (model * vec4(position, 1.0)).xyz;
            vertex_data.texture_coordinates = position;
            vertex_data.normal = normalize((model * vec4(normal, 1.0)).xyz);

            gl_Position = projection * view * model * vec4(position.xyz, 1.0);
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 450 core

        in VertexData {
            vec3 position;
            vec3 texture_coordinates;
            vec3 normal;
        } vertex_data;

        out vec4 fragment_color;

        layout(binding=0) uniform samplerCube texture_sampler;
        uniform vec4 color;
        uniform float ambient_strength;
        uniform vec3 light_source_position;
        uniform vec3 light_color;

        void main() {
            vec4 ambient_color = vec4(vec3(ambient_strength), 1);

            vec3 light_direction = normalize(vec3(light_source_position - vertex_data.position));
            vec3 diffuse = light_color * max(dot(light_direction, vertex_data.normal), 0.0f);

            vec4 diffuse_color = vec4(diffuse, 1);

            fragment_color = ambient_color * diffuse_color * texture(texture_sampler, vertex_data.texture_coordinates) * color;
        }
    )";

    auto cubeShader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    // Chessboard mesh
    auto cubeVertices =
        framework::generateUnitCubeWithNormals() | std::views::transform([](auto vertex) {
            return Cube::Vertex{
                .position = vertex.position,
                .normal = vertex.normal
            };
        });

    // Diffuse illumination
    cubeShader->uploadUniformFloat3("light_source_position", camera.position);
    cubeShader->uploadUniformFloat3("light_color", {1.5, 1.5, 1.});

    // Transformation, model is set in draw()
    cubeShader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    cubeShader->uploadUniformMatrix4("view", camera.viewMatrix);

    auto object = framework::createVertexArrayObject<Cube::Vertex>(
        cubeShader,
        {
            {.type =GL_FLOAT, .size = 3, .offset = offsetof(Cube::Vertex, position)},
            {.type =GL_FLOAT, .size = 3, .offset = offsetof(Cube::Vertex, normal)},
        },
        {cubeVertices.begin(), cubeVertices.end()},
        {}
    );

    auto texture = framework::loadCubemap(TEXTURES_DIR + std::string("concrete.png"));

    return {
        .window = window,
        .object = object,
        .texture = std::move(texture)
    };
}