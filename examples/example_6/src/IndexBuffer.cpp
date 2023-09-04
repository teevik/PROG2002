#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(std::vector<uint32_t> buffer) : buffer(buffer), size(buffer.size()) {
    glGenBuffers(1, &rendererId);
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(uint32_t), buffer.data(), GL_STATIC_DRAW);
    this->unbind();
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &rendererId);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}