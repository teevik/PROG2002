#ifndef PROG2002_INDEXBUFFER_H
#define PROG2002_INDEXBUFFER_H

#include <cstddef>
#include <vector>
#include "glad/glad.h"

class IndexBuffer {
private:
    uint32_t rendererId = 0;
    const std::vector<uint32_t> buffer;

public:
    const size_t size;

    explicit IndexBuffer(std::vector<uint32_t> buffer);

    ~IndexBuffer();

    void bind() const;

    void unbind() const;
};

#endif //PROG2002_INDEXBUFFER_H
