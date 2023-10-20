#ifndef PROG2002_UNIFORMBUFFER_H
#define PROG2002_UNIFORMBUFFER_H

#include <vector>
#include "glad/glad.h"

namespace framework {

    /**
     * An uniform buffer object
     */
    template<typename T>
    struct UniformBuffer {
        uint32_t uniformBufferId;

        explicit UniformBuffer(uint32_t uniformBufferId) : uniformBufferId(uniformBufferId) {};

        UniformBuffer(UniformBuffer &&object) noexcept: uniformBufferId(object.uniformBufferId) {
            object.uniformBufferId = 0;
        }

        ~UniformBuffer() {
            if (uniformBufferId) glDeleteBuffers(1, &uniformBufferId);
        }

        void updateData(const std::vector<T> &data) const {
            glNamedBufferData(uniformBufferId, data.size() * sizeof(T), data.data(), GL_DYNAMIC_DRAW);
        }

        static UniformBuffer<T> create(
            const std::vector<T> &data
        ) {
            uint32_t uniformBufferId;
            glCreateBuffers(1, &uniformBufferId);
            glNamedBufferData(uniformBufferId, data.size() * sizeof(T), data.data(), GL_DYNAMIC_DRAW);

            return UniformBuffer<T>(uniformBufferId);
        }
    };
}

#endif //PROG2002_UNIFORMBUFFER_H
