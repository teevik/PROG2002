#ifndef PROG2002_GEOMETRY_H
#define PROG2002_GEOMETRY_H

#include "Object.h"
#include "glm/vec2.hpp"

namespace framework {
    constexpr framework::Triangle<glm::vec2> unitTriangle2d = {
        .a = {-0.5f, -0.5f},
        .b = {0.5f, -0.5f},
        .c = {0.0f, 0.5f}
    };

    /**
     * Generate a unit circle mesh
     * @param resolution amount of triangles
     */
    std::vector<framework::Triangle<glm::vec2>> generateCircleMesh(int resolution);
}

#endif //PROG2002_GEOMETRY_H