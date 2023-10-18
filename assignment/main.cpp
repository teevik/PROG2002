#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"

#include "ChessBoard.h"
#include "ChessPieces.h"

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Assignment");

    // Camera
    float aspectRatio = (float) width / (float) height;
    glm::vec3 position = {0.f, 0.f, 5.f};
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 1.f, 0.1f};

    auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    // Objects
    auto chessboard = ChessBoard::create();
    auto chessPieces = ChessPieces::create();

    // State
    static glm::ivec2 selectedTile;
    static bool useTextures = true;

    // Handle input
    auto handleKeyInput = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_T:
                    useTextures = !useTextures;
                    break;

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
    };
    glfwSetKeyCallback(window, handleKeyInput);

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Clear color
    glm::vec3 backgroundColor = {0.917f, 0.905f, 0.850f};

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Background color
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

        // GLFW events
        glfwPollEvents();

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chessboard.draw(selectedTile, useTextures, camera);
        chessPieces.draw(useTextures, camera);

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}