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

    // Needs to comply with std140, so each data needs to be 16 bytes long
    struct InstanceData {
        glm::ivec2 position; // 8 bytes
        glm::vec2 _padding1; //8 of padding

        glm::vec4 color; // 16 bytes

        bool isBeingMoved; // 4 bytes
        glm::vec3 _padding2; // 12 of padding
    };

    const framework::VertexArray<Vertex> object;
    const framework::Texture texture;
    const framework::UniformBuffer<InstanceData> instanceBuffer;

    static ChessPieces create(const std::vector<InstanceData> &pieces);

    void updatePieces(const std::vector<InstanceData> &pieces) const;

    void draw(bool useTextures, const framework::Camera &camera) const;
};

#endif //PROG2002_CHESSPIECES_H
