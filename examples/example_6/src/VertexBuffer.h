#ifndef PROG2002_VERTEXBUFFER_H
#define PROG2002_VERTEXBUFFER_H

#include <cstddef>
#include <vector>
#include "glad/glad.h"

struct BufferLayout {
    /// The number of components, must be 1, 2, 3, or 4
    int32_t size;

    /// Specifies the data type of each component in the array
    GLenum type;

    /// Specifies whether fixed-point data values should be normalized
    bool normalized;

    /// Specifies the byte offset between consecutive generic vertex attributes
    int32_t stride;
};

class VertexBuffer {
private:
    uint32_t rendererId = 0;
    const std::vector<float> buffer;

public:
    const size_t size;
    const BufferLayout layout;

    explicit VertexBuffer(std::vector<float> buffer, BufferLayout layout);

    ~VertexBuffer();

    void bind() const;

    void unbind() const;
};

#endif //PROG2002_VERTEXBUFFER_H
