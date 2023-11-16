#include <iostream>
#include "framework/window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

static void glfwErrorCallback(int32_t code, const char *description) {
    std::cerr << "GLFW Error (0x" << std::hex << code << "): " << description << std::endl;
}

std::string getSeverityString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return "High";

        case GL_DEBUG_SEVERITY_MEDIUM :
            return "Medium";

        case GL_DEBUG_SEVERITY_LOW :
            return "Low";

        case GL_DEBUG_SEVERITY_NOTIFICATION :
            return "Notification";

        default:
            return "Unknown";
    }
}

std::string getTypeString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_TYPE_ERROR :
            return "Error";

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR :
            return "Deprecated behavior";

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR :
            return "Undefined behavior";

        case GL_DEBUG_TYPE_PORTABILITY :
            return "Not portable";

        case GL_DEBUG_TYPE_PERFORMANCE :
            return "Performance issue";

        case GL_DEBUG_TYPE_MARKER :
            return "Command stream annotation";

        case GL_DEBUG_TYPE_PUSH_GROUP :
            return "Group pushing";

        case GL_DEBUG_TYPE_POP_GROUP :
            return "Group popping";

        case GL_DEBUG_TYPE_OTHER  :
            return "Other";

        default:
            return "Unknown";
    }
}

static void GLAPIENTRY debugMessageCallback(
    [[maybe_unused]] GLenum source,
    GLenum type,
    [[maybe_unused]] GLuint id,
    GLenum severity,
    [[maybe_unused]] GLsizei length,
    const GLchar *message,
    [[maybe_unused]] const void *userParam
) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        throw std::runtime_error(message);
    } else {
        std::ostream &output = severity == GL_DEBUG_SEVERITY_NOTIFICATION ? std::cout : std::cerr;

        output << "OpenGL Callback: "
               << " Type: " << getTypeString(type)
               << ", Severity: " << getSeverityString(severity)
               << ", Message: " << message << std::endl;
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
        std::cout << std::endl;

        return window;
    }
}
