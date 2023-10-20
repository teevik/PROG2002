#ifndef PROG2002_GEOMETRY_H
#define PROG2002_GEOMETRY_H

#include "VertexArray.h"
#include "glm/vec2.hpp"

namespace framework {
    struct VertexWithNormal {
        glm::vec3 position;
        glm::vec3 normal;
    };

    static const std::vector<glm::vec2> unitTriangle = {
        {-0.5f, -0.5f},
        {0.5f,  -0.5f},
        {0.0f,  0.5f}
    };

    static const std::vector<glm::vec2> quad = {
        {1.f,  1.f}, // top right
        {1.f,  -1.f}, // bottom right
        {-1.f, 1.f}, // top left
        {1.f,  -1.f}, // bottom right
        {-1.f, -1.f}, // bottom left
        {-1.f, 1.f} // top left
    };

    namespace unitCube {
        const std::vector<glm::vec3> vertices = {
            // front
            {-1.0, -1.0, 1.0},
            {1.0,  -1.0, 1.0},
            {1.0,  1.0,  1.0},
            {-1.0, 1.0,  1.0},
            // back
            {-1.0, -1.0, -1.0},
            {1.0,  -1.0, -1.0},
            {1.0,  1.0,  -1.0},
            {-1.0, 1.0,  -1.0}
        };

        const std::vector<uint32_t> indices = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
        };
    }

    std::vector<VertexWithNormal> generateUnitCubeWithNormals();

    /**
     * Generate a unit circle mesh
     * @param resolution amount of triangles
     */
    std::vector<glm::vec2> generateCircleMesh(int resolution);

    /// Mesh with indices
    struct IndexMesh {
        std::vector<glm::vec2> vertices;
        std::vector<uint32_t> indices;
    };

    IndexMesh generateGridMesh(int resolution);
}

#endif //PROG2002_GEOMETRY_H
