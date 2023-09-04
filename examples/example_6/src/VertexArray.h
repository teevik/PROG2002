#ifndef PROG2002_VERTEXARRAY_H
#define PROG2002_VERTEXARRAY_H

#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
private:
    uint32_t rendererId = 0;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;

public:
    VertexArray(
            std::shared_ptr<VertexBuffer> vertexBuffer,
            std::shared_ptr<IndexBuffer> indexBuffer
    );

    VertexArray(std::vector<float> vertices, std::vector<uint32_t> indices, BufferLayout vertexBufferLayout);

    ~VertexArray();

    void bind() const;

    void unbind() const;

    void draw() const;
};

#endif //PROG2002_VERTEXARRAY_H
