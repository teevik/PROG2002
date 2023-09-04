#include "VertexArray.h"

VertexArray::VertexArray(
        std::shared_ptr<VertexBuffer> vertexBuffer,
        std::shared_ptr<IndexBuffer> indexBuffer
) : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {
    glGenVertexArrays(1, &rendererId);

    this->bind();
    vertexBuffer->bind();

    auto &layout = vertexBuffer->layout;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,
            layout.size,
            layout.type,
            layout.normalized,
            layout.stride,
            nullptr
    );

    vertexBuffer->unbind();
    this->unbind();
}

VertexArray::VertexArray(std::vector<float> vertices, std::vector<uint32_t> indices, BufferLayout vertexBufferLayout)
        : VertexArray(
        (std::shared_ptr<VertexBuffer>) new VertexBuffer(vertices, vertexBufferLayout),
        (std::shared_ptr<IndexBuffer>) new IndexBuffer(indices)
) {

}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &rendererId);
}

void VertexArray::bind() const {
    glBindVertexArray(rendererId);

    this->indexBuffer->bind();
}

void VertexArray::unbind() const {
    this->indexBuffer->unbind();

    glBindVertexArray(0);
}

void VertexArray::draw() const {
    this->bind();
    glDrawElements(GL_TRIANGLES, this->indexBuffer->size, GL_UNSIGNED_INT, nullptr);
    this->unbind();
}