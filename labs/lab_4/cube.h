#ifndef PROG2002_CUBE_H
#define PROG2002_CUBE_H

#include "glm/gtx/euler_angles.hpp"
#include "glm/vec3.hpp"
#include "framework/VertexArrayObject.h"
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
    };

    GLFWwindow *window;
    const framework::VertexArrayObject<Vertex> object;
    const framework::Texture texture;

    void draw() const;
};

Cube createCube(GLFWwindow *window, framework::Camera camera);

#endif //PROG2002_CUBE_H
