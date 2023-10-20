#ifndef PROG2002_CHESSPIECES_H
#define PROG2002_CHESSPIECES_H

#include "glm/vec3.hpp"
#include "framework/VertexArray.h"
#include "framework/Texture.h"
#include "framework/Camera.h"

struct ChessPieces {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;
    };

    struct InstanceData {
        glm::ivec2 piecePositions;
        glm::vec2 _padding; // Padding to comply with std140
        glm::vec4 color;
    };

    const framework::VertexArray<Vertex> object;
    const framework::Texture texture;
    const std::vector<InstanceData> instances;
    const uint32_t instanceBufferId;

    static ChessPieces create();

    void draw(bool useTextures, framework::Camera camera) const;
};

#endif //PROG2002_CHESSPIECES_H
