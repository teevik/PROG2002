#include <iostream>
#include "framework/window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

static void glfwErrorCallback(int32_t code, const char *description) {
    std::cerr << "GLFW Error (0x" << std::hex << code << "): " << description << std::endl;
}

static void GLAPIENTRY debugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
) {
    std::cerr << "OpenGL Callback: " << (type == GL_DEBUG_TYPE_ERROR ? "** ERROR **" : "")
              << " Type: 0x" << type
              << ", Severity: 0x" << severity
              << ", Message: " << message << std::endl;

    if (type == GL_DEBUG_TYPE_ERROR) {
        throw std::runtime_error(message);
    }
}

namespace framework {
    GLFWwindow *createWindow(int width, int height, const std::string &title) {
        glfwSetErrorCallback(glfwErrorCallback);

        auto didInitializeGlfw = glfwInit();
        if (!didInitializeGlfw) {
            std::cerr << "Failed to initialize GLFW" << std::endl;

            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_RESIZABLE, false);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        auto window = glfwCreateWindow(
            width,
            height,
            title.c_str(),
            nullptr,
            nullptr
        );

        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();

            exit(EXIT_FAILURE);
        }

        // Set OpenGL context
        glfwMakeContextCurrent(window);

        auto didInitializeGlad = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        if (!didInitializeGlad) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();

            exit(EXIT_FAILURE);
        }

        // Enable OpenGL debug output
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugMessageCallback, nullptr);

        // Print OpenGL information
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        return window;
    }
}
