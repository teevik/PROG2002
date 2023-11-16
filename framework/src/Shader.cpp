#include "framework/Shader.h"
#include "glad/glad.h"
#include <memory>
#include <iostream>
#include <optional>
#include <cassert>

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
uint32_t createShaderPipeline(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
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

namespace framework {
    Shader::Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) :
        id(createShaderPipeline(vertexShaderSource, fragmentShaderSource)) {
    }

    Shader::Shader(Shader &&shader) noexcept: id(shader.id) {
        shader.id = 0;
    }

    Shader::~Shader() {
        if (id) glDeleteProgram(id);
    }

    void Shader::uploadUniformBool1(const std::string &name, bool value) const {
        int32_t location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform1i(id, location, value);
    }

    void Shader::uploadUniformInt1(const std::string &name, int value) const {
        int32_t location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform1i(id, location, value);
    }

    void Shader::uploadUniformInt2(const std::string &name, glm::ivec2 value) const {
        auto location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform2i(id, location, value.x, value.y);
    }

    void Shader::uploadUniformFloat1(const std::string &name, float value) const {
        int32_t location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform1f(id, location, value);
    }

    void Shader::uploadUniformFloat3(const std::string &name, glm::vec3 value) const {
        int32_t location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform3f(id, location, value.r, value.g, value.b);
    }

    void Shader::uploadUniformFloat4(const std::string &name, glm::vec4 value) const {
        int32_t location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniform4f(id, location, value.r, value.g, value.b, value.a);
    }

    void Shader::uploadUniformMatrix4(const std::string &name, glm::mat4 value) const {
        auto location = glGetUniformLocation(id, name.c_str());
        assert(location != -1);
        glProgramUniformMatrix4fv(id, location, 1, false, &value[0][0]);
    }
}