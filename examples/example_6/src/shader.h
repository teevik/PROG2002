#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

#include <cstdint>
#include <optional>
#include <string>
#include "glad/glad.h"

std::optional<unsigned int> compileShader(const std::string &source, GLenum shaderType);

uint32_t createShaderPipeline(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

#endif //PROG2002_SHADER_H
