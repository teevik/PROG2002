#ifndef PROG2002_CHESSBOARD_H
#define PROG2002_CHESSBOARD_H

#include "glm/ext/vector_int2.hpp"
#include "glm/vec2.hpp"
#include "framework/VertexArray.h"
#include "framework/Texture.h"
#include "framework/Camera.h"

struct ChessBoard {
    struct Vertex {
        /// Vertex position
        glm::vec2 position;

        /// Texture coordinate
        glm::vec2 textureCoordinates;

        /// Position between {0.f, 0.f} (top left corner) and {1.f, 1.f} (bottom right corner)
        glm::vec2 gridPosition;
    };

    const framework::VertexArray<Vertex> vertexArray;
    const framework::Texture texture;

    static ChessBoard create();

    void draw(glm::ivec2 selectedTile, bool useTextures, const framework::Camera &camera) const;
};

#endif //PROG2002_CHESSBOARD_H
