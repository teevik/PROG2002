#ifndef PROG2002_WINDOW_H
#define PROG2002_WINDOW_H

#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace framework {
    GLFWwindow *createWindow(int width, int height, const std::string &title);
}

#endif //PROG2002_WINDOW_H
