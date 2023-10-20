#ifndef PROG2002_CUBE_H
#define PROG2002_CUBE_H

#include "glm/gtx/euler_angles.hpp"
#include "glm/vec3.hpp"
#include "framework/VertexArray.h"
#include "framework/Texture.h"
#include "glm/ext/matrix_transform.hpp"
#include "framework/geometry.h"
#include "glm/gtx/euler_angles.hpp"
#include "GLFW/glfw3.h"
#include "framework/Camera.h"

struct Cube {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;

        /// Vertex position
        glm::vec3 normal;
    };

    GLFWwindow *window;
    const framework::VertexArray<Vertex> object;
    const framework::Texture texture;

    static Cube create(GLFWwindow *window, framework::Camera camera);

    void draw(float ambientStrength) const;
};

#endif //PROG2002_CUBE_H
