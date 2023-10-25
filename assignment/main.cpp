#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "ChessBoard.h"
#include "ChessPieces.h"
#include "constants.h"

static std::vector<ChessPieces::InstanceData> initialChessPieces() {
    std::vector<ChessPieces::InstanceData> pieces;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < TEAM_ROWS; ++y) {
            pieces.push_back(
                {
                    .position = {x, y},
                    .color = {1., 0., 0., 1.}
                }
            );
        }

        for (int y = BOARD_SIZE - TEAM_ROWS; y < BOARD_SIZE; ++y) {
            pieces.push_back(
                {
                    .position = {x, y},
                    .color = {0., 0., 1., 1.}
                }
            );
        }
    }

    return pieces;
}

glm::vec3 calculateCameraPosition(float angle, float zoom) {
    glm::vec3 position = {4.f * glm::cos(angle) * zoom, 4.f * glm::sin(angle) * zoom, 1.8f * zoom};

    return position;
}

int main() {
    int width = 800;
    int height = 600;

    auto window = framework::createWindow(width, height, "Assignment");

    // State
    static float cameraAngle = glm::pi<float>() * 1.5f;
    static float zoom = 1.f;
    static bool useTextures = true;
    static glm::ivec2 selectedTile = {0, 0};
    static std::optional<glm::ivec2> pieceBeingMoved = {};
    static auto pieces = initialChessPieces();

    // Camera
    float aspectRatio = (float) width / (float) height;
    glm::vec3 position = calculateCameraPosition(cameraAngle, zoom);
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 0.f, 1.f};

    static auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    // Objects
    auto chessboard = ChessBoard::create();
    auto static chessPieces = ChessPieces::create(pieces);

    // Time
    double lastFrameTime;
    static float deltaTime;

    // Update function
    auto updateGame = [window]() {
        if (glfwGetKey(window, GLFW_KEY_L)) {
            cameraAngle += CAMERA_SENSITIVITY * deltaTime;
            camera.position = calculateCameraPosition(cameraAngle, zoom);
        }

        if (glfwGetKey(window, GLFW_KEY_H)) {
            cameraAngle -= CAMERA_SENSITIVITY * deltaTime;
            camera.position = calculateCameraPosition(cameraAngle, zoom);
        }

        if (glfwGetKey(window, GLFW_KEY_P)) {
            zoom -= ZOOM_SENSITIVITY * deltaTime;
            zoom = glm::clamp(zoom, MIN_ZOOM, MAX_ZOOM);
            camera.position = calculateCameraPosition(cameraAngle, zoom);
        }

        if (glfwGetKey(window, GLFW_KEY_O)) {
            zoom += ZOOM_SENSITIVITY * deltaTime;
            zoom = glm::clamp(zoom, MIN_ZOOM, MAX_ZOOM);
            camera.position = calculateCameraPosition(cameraAngle, zoom);
        }
    };

    // Handle input
    auto handleKeyInput = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                // Toggle textures
                case GLFW_KEY_T:
                    useTextures = !useTextures;
                    break;

                    // Tile selection move
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

                    // Tile select
                case GLFW_KEY_ENTER: {
                    auto pieceAtSelectedTile = std::ranges::find_if(pieces, [](const ChessPieces::InstanceData &piece) {
                        return piece.position == selectedTile;
                    });
                    bool selectedTileHasExistingPiece = pieceAtSelectedTile != pieces.end();

                    if (!pieceBeingMoved.has_value()) {
                        // Nothing is being moved

                        if (selectedTileHasExistingPiece) {
                            pieceBeingMoved = selectedTile;
                        }
                    } else {
                        // A piece is being moved
                        auto movedPiece = std::ranges::find_if(
                            pieces,
                            [](const ChessPieces::InstanceData &piece) {
                                return piece.position == pieceBeingMoved;
                            }
                        );

                        if (!selectedTileHasExistingPiece) {
                            // Can move to selected tile
                            movedPiece->position = selectedTile;
                        }

                        pieceBeingMoved = {};
                        chessPieces.updatePieces(pieces);
                    }
                    break;
                }

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
        // Set deltaTime
        auto time = glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        // Update
        glfwPollEvents();
        updateGame();

        // Background color
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chessboard.draw(selectedTile, useTextures, camera);
        chessPieces.draw(selectedTile, pieceBeingMoved, useTextures, camera);

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}