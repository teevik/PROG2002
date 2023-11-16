#ifndef PROG2002_INDEXBUFFER_H
#define PROG2002_INDEXBUFFER_H

#include <cstdint>
#include <vector>
#include "glad/glad.h"

namespace framework {
    /// OpenGL index buffer
    struct IndexBuffer {
        /// Type used to store indices
        using IndexType = uint32_t;

        const uint32_t elementsAmount;
        uint32_t indexBufferId = 0;

        explicit IndexBuffer(
            std::vector<IndexType> indices
        );

        // Move constructor
        IndexBuffer(IndexBuffer &&object) noexcept;

        ~IndexBuffer();
    };
}

#endif //PROG2002_INDEXBUFFER_H
