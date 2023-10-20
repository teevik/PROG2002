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
    const framework::UniformBuffer<InstanceData> instanceBuffer;

    static ChessPieces create(const std::vector<InstanceData> &pieces);

    void updatePieces(const std::vector<InstanceData> &pieces) const;

    void draw(bool useTextures, const framework::Camera &camera) const;
};

#endif //PROG2002_CHESSPIECES_H
