#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "Renderer.h"

int main() {
    auto window = createWindow();

    Renderer renderer;

    glm::vec4 white = {1.f, 1.f, 1.f, 1.f};

    auto triangle = Triangle{
            .a = {.position = glm::vec2(-0.5f, -0.5f), .color = white},
            .b = {.position = glm::vec2(0.5f, -0.5f), .color = white},
            .c = {.position = glm::vec2(0.5f, 0.5f), .color = white}
    };

    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
    renderer.initalize();

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.beginFrame();
        renderer.pushTriangle(triangle);
        renderer.endFrame();

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    renderer.free();
    glfwTerminate();

    return EXIT_SUCCESS;
}