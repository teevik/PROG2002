#define GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "stb_image.h"
#include "framework/window.h"
#include "chessboard.h"
#include "cube.h"
#include "framework/Camera.h"

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Lab 4");

    // Camera
    float aspectRatio = (float) width / (float) height;
    glm::vec3 position = {0.f, 0.f, 5.f};
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 1.f, 0.1f};

    auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    // TODO: Static to access in callback, probably bad hack?
    static auto chessboard = createChessboard(camera);
    auto cube = createCube(window, camera);

    // Handle input
    auto keyCallback = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        chessboard.handleKeyInput(key, action);
    };

    glfwSetKeyCallback(window, keyCallback);

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    chessboard.free();

    glfwTerminate();

    return EXIT_SUCCESS;
}