#include "geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

StaticMesh<Vertex> generateCircleMesh(glm::vec2 center, int resolution) {
    std::vector<Triangle<Vertex>> circleTriangles;

    for (int i = 0; i < resolution; i++) {
        float valueStart = (float) i / (float) resolution;
        float valueEnd = ((float) (i + 1) / (float) resolution);

        float angleStart = valueStart * glm::pi<float>() * 2.f;
        float angleEnd = valueEnd * glm::pi<float>() * 2.f;

        circleTriangles.push_back(
            {
                .a =   {
                    .position = center + glm::vec2(glm::cos(angleStart), glm::sin(angleStart)),
                    .color = {valueStart, valueStart, valueStart, 1.f}
                },
                .b =   {
                    .position = center + glm::vec2(glm::cos(angleEnd), glm::sin(angleEnd)),
                    .color = {valueEnd, valueEnd, valueEnd, 1.f}
                },
                .c =   {
                    .position = center,
                    .color = {1.f, 0.f, 1.f, 1.f}
                },
            }
        );
    }

    return StaticMesh<Vertex>{.triangles = std::move(circleTriangles)};
}
