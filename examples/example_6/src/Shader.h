#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H


#include <string>

class Shader {
public:
    const uint32_t id;

    Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    ~Shader();

    Shader(const Shader &) = delete;

    Shader &operator=(const Shader &) = delete;
};


#endif //PROG2002_SHADER_H
