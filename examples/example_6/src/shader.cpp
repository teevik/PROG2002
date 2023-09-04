#include <memory>
#include <iostream>
#include "shader.h"

/**
 * @param source
 * @param shaderType
 * @return shaderId if successful
 */
std::optional<uint32_t> compileShader(const std::string &source, GLenum shaderType) {
    uint32_t shaderId = glCreateShader(shaderType);

    const char *rawSource = source.c_str();
    glShaderSource(shaderId, 1, &rawSource, nullptr);
    glCompileShader(shaderId);

    int32_t shaderDidCompile;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderDidCompile);
    if (!shaderDidCompile) {
        int32_t errorLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);

        auto errorMessage = std::make_unique<char[]>(errorLength);
        glGetShaderInfoLog(shaderId, errorLength, &errorLength, errorMessage.get());

        std::cerr << "Failed to compile shader!" << std::endl;
        std::cerr << errorMessage.get() << std::endl;

        glDeleteShader(shaderId);

        return std::nullopt;
    }

    return shaderId;
}

/**
 * @param vertexShaderSource
 * @param fragmentShaderSource
 * @return Shader program
 */
uint32_t createShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    uint32_t program = glCreateProgram();

    uint32_t vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER).value();
    uint32_t fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER).value();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
