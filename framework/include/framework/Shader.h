#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

#include <string>
#include "glm/ext/matrix_float4x4.hpp"

namespace framework {
    class Shader {
    public:
        const uint32_t id;

        Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        ~Shader();

        Shader(const Shader &) = delete;

        Shader &operator=(const Shader &) = delete;

        void uploadUniformInt1(const std::string &name, int value) const;

        void uploadUniformInt2(const std::string &name, glm::ivec2 value) const;

        void uploadUniformFloat1(const std::string &name, float value) const;

        void uploadUniformMatrix4(const std::string &name, glm::mat4 value) const;

    };
}

#endif //PROG2002_SHADER_H
