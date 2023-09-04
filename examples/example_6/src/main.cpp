#include <string>
#include <cassert>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "VertexArray.h"
#include "shader.h"

int main() {
    auto window = createWindow();

    auto vertexArray1 = VertexArray(
            {
                    -0.5f, -0.5f,
                    0.5f, -0.5f,
                    0.5f, 0.5f,
                    -0.5f, 0.5f,
            },
            {
                    0, 1, 2,
                    2, 3, 0
            },
            {.size=2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(float) * 2}
    );

    auto vertexArray2 = VertexArray(
            {
                    -0.4f, -0.6f,
                    0.4f, -0.6f,
                    0.4f, 0.6f,
                    -0.4f, 0.6f,
            },
            {
                    0, 1, 2,
                    2, 3, 0
            },
            {.size=2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(float) * 2}
    );


    // language=glsl
    const std::string vertexShaderSource = R"(
        #version 430 core

        layout(location = 0) in vec4 position;

        void main() {
            gl_Position = position;
        }
    )";

    // language=glsl
    const std::string fragmentShaderSource = R"(
        #version 430 core

        float rand(float n) { return fract(sin(n) * 43758.5453123); }

        float noise(float p) {
            float fl = floor(p);
            float fc = fract(p);
            return mix(rand(fl), rand(fl + 1.0), fc);
        }

        uniform float time;

        out vec4 color;

        void main() {
            color = vec4(noise(time), noise(time + 100), noise(time + 200), 1);
        }
    )";

    uint32_t shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set time uniform
        auto time = (float) glfwGetTime();
        int32_t timeLocation = glGetUniformLocation(shaderProgram, "time");
        assert(timeLocation != -1);
        glUniform1f(timeLocation, time);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        vertexArray1.draw();
        vertexArray2.draw();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }


    glfwTerminate();

    return EXIT_SUCCESS;
}