#ifndef PROG2002_CHESSPIECES_H
#define PROG2002_CHESSPIECES_H

#include "glm/vec3.hpp"
#include "framework/VertexArrayObject.h"
#include "framework/Texture.h"
#include "framework/Camera.h"

struct ChessPieces {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;
    };

    const framework::VertexArrayObject<Vertex> object;
    const framework::Texture texture;

    static ChessPieces create();

    void draw(bool useTextures, framework::Camera camera) const;
};

#endif //PROG2002_CHESSPIECES_H
