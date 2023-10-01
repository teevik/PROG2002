#ifndef PROG2002_CHESSBOARD_H
#define PROG2002_CHESSBOARD_H

#include "glm/ext/vector_int2.hpp"
#include "glm/vec2.hpp"
#include "framework/VertexArrayObject.h"
#include "framework/Texture.h"

const int BOARD_SIZE = 8;

struct Chessboard {
    struct Vertex {
        /// Vertex position
        glm::vec2 position;

        /// Texture coordinate
        glm::vec2 textureCoordinates;

        /// Position between {0.f, 0.f} (top left corner) and {1.f, 1.f} (bottom right corner)
        glm::vec2 gridPosition;
    };

    const framework::VertexArrayObject<Vertex> object;
    const framework::Texture texture;

    glm::ivec2 selectedTile;

    void draw() const;

    void handleKeyInput(int key, int action);

    void free() {
        object.free();
        texture.free();
    }
};

Chessboard createChessboard(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

#endif //PROG2002_CHESSBOARD_H
