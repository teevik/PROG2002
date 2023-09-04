#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(
        std::vector<float> buffer,
        BufferLayout layout
) : buffer(buffer), size(buffer.size()), layout(layout) {
    glGenBuffers(1, &rendererId);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);
    this->unbind();
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &rendererId);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}