#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "ChessBoard.h"
#include "ChessPieces.h"
#include "constants.h"

/// Initial chess pieces
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

/// Find camera position that orbits around origin given `angle` and `zoom`,
glm::vec3 calculateCameraPosition(float angle, float zoom) {
    glm::vec3 position = {4.f * glm::cos(angle) * zoom, 4.f * glm::sin(angle) * zoom, 1.8f * zoom};

    return position;
}

struct GameState {
    /// Camera angle
    float cameraAngle;

    /// Camera zoom
    float cameraZoom;

    /// Whether to render with textures or not
    bool useTextures;

    /// The position of the tile that is currently being hovered by the player
    glm::ivec2 selectedTile;

    /// The position of the piece that is currently being moved, empty if not moving one
    std::optional<glm::ivec2> pieceBeingMoved;

    /// Position and color of each chess piece, can be directly loaded into a UniformBuffer
    std::vector<ChessPieces::InstanceData> pieces;

    /// Whether pieces has been changed, will need to upload to the UniformBuffer again
    bool piecesHasUpdated;

    /// Handle key input from GLFW
    void handleKeyInput(int key, int action) {
        if (action != GLFW_PRESS) return;

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
                auto pieceAtSelectedTile = std::ranges::find_if(
                    pieces,
                    [this]
                        (const ChessPieces::InstanceData &piece) {
                        return piece.position == selectedTile;
                    }
                );
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
                        [this](const ChessPieces::InstanceData &piece) {
                            return piece.position == pieceBeingMoved;
                        }
                    );

                    if (!selectedTileHasExistingPiece) {
                        // Can move to selected tile
                        movedPiece->position = selectedTile;
                    }

                    pieceBeingMoved = {};
                    piecesHasUpdated = true;
                }
                break;
            }

            default:
                break;
        }
    }

    /// Game loop update
    void update(GLFWwindow *window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_L)) {
            cameraAngle += CAMERA_SENSITIVITY * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_H)) {
            cameraAngle -= CAMERA_SENSITIVITY * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_P)) {
            cameraZoom -= ZOOM_SENSITIVITY * deltaTime;
            cameraZoom = glm::clamp(cameraZoom, MIN_ZOOM, MAX_ZOOM);
        }

        if (glfwGetKey(window, GLFW_KEY_O)) {
            cameraZoom += ZOOM_SENSITIVITY * deltaTime;
            cameraZoom = glm::clamp(cameraZoom, MIN_ZOOM, MAX_ZOOM);
        }
    };

};


int main() {
    int width = 800;
    int height = 600;
    float aspectRatio = (float) width / (float) height;

    auto window = framework::createWindow(width, height, "Assignment");

    // Game state, only static so that it can be used in glfwSetKeyCallback
    static GameState gameState = {
        .cameraAngle = glm::pi<float>() * 1.5f,
        .cameraZoom = 1.f,
        .useTextures = true,
        .selectedTile = {0, 0},
        .pieceBeingMoved = {},
        .pieces = initialChessPieces()
    };

    // Camera
    glm::vec3 position = calculateCameraPosition(gameState.cameraAngle, gameState.cameraZoom);
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 0.f, 1.f};

    static auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    // Objects
    auto chessboard = ChessBoard::create();
    auto chessPieces = ChessPieces::create(gameState.pieces);

    // Time
    double lastFrameTime;
    float deltaTime;

    // Handle input
    auto handleKeyInput = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        gameState.handleKeyInput(key, action);
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
        deltaTime = (float) (time - lastFrameTime);
        lastFrameTime = time;

        // Update
        glfwPollEvents();
        gameState.update(window, deltaTime);
        camera.position = calculateCameraPosition(gameState.cameraAngle, gameState.cameraZoom);

        if (gameState.piecesHasUpdated) {
            chessPieces.updatePieces(gameState.pieces);
            gameState.piecesHasUpdated = false;
        }

        // Background color
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        chessboard.draw(gameState.selectedTile, gameState.useTextures, camera);
        chessPieces.draw(gameState.selectedTile, gameState.pieceBeingMoved, gameState.useTextures, camera);

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape button
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}