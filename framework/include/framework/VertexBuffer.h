#ifndef PROG2002_VERTEXBUFFER_H
#define PROG2002_VERTEXBUFFER_H

#include <cstdint>
#include <vector>
#include "glad/glad.h"

namespace framework {
    /// OpenGL vertex buffer
    template<typename VertexType>
    struct VertexBuffer {
        const uint32_t verticesAmount;
        uint32_t vertexBufferId = 0;

        explicit VertexBuffer(std::vector<VertexType> vertices) : verticesAmount(vertices.size()) {
            glCreateBuffers(1, &vertexBufferId);

            glNamedBufferData(
                vertexBufferId,
                vertices.size() * sizeof(VertexType),
                vertices.data(),
                GL_STATIC_DRAW
            );
        };

        // Move constructor
        VertexBuffer(VertexBuffer &&object) noexcept:
            verticesAmount(object.verticesAmount),
            vertexBufferId(object.vertexBufferId) {
            object.vertexBufferId = 0;
        }

        ~VertexBuffer() {
            if (vertexBufferId) glDeleteBuffers(1, &vertexBufferId);
        }
    };
}

#endif //PROG2002_VERTEXBUFFER_H
