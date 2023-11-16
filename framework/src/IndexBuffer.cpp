#include "framework/IndexBuffer.h"

namespace framework {
    IndexBuffer::IndexBuffer(std::vector<IndexType> indices) : elementsAmount(indices.size()) {
        glCreateBuffers(1, &indexBufferId);

        glNamedBufferData(
            indexBufferId,
            indices.size() * sizeof(IndexType),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

    IndexBuffer::IndexBuffer(IndexBuffer &&object) noexcept:
        elementsAmount(object.elementsAmount),
        indexBufferId(object.indexBufferId) {
        object.indexBufferId = 0;
    }

    IndexBuffer::~IndexBuffer() {
        if (indexBufferId) glDeleteBuffers(1, &indexBufferId);
    }
}