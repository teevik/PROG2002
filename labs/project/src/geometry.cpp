#include "geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

StaticMesh<Vertex> generateCircleMesh(glm::vec2 center, int resolution) {
    std::vector<Triangle<Vertex>> circleTriangles;

    glm::vec4 color{1.f, 1.f, 1.f, 1.f};

    for (int i = 0; i < resolution; i++) {
        float valueStart = (float) i / (float) resolution;
        float valueEnd = ((float) (i + 1) / (float) resolution);

        float angleStart = valueStart * glm::pi<float>() * 2.f;
        float angleEnd = valueEnd * glm::pi<float>() * 2.f;

        circleTriangles.push_back(
            {
                .a = {
                    .position = center + glm::vec2(glm::cos(angleStart), glm::sin(angleStart)),
                    .color = color
                },
                .b = {
                    .position = center + glm::vec2(glm::cos(angleEnd), glm::sin(angleEnd)),
                    .color = color
                },
                .c = {
                    .position = center,
                    .color = color
                }
            }
        );
    }

    return StaticMesh<Vertex>{.triangles = std::move(circleTriangles)};
}