#include "framework/geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

namespace framework {
    std::vector<VertexWithNormal> generateUnitCubeWithNormals() {
        std::vector<VertexWithNormal> mesh;

        for (int i = 0; i < unitCube.indices.size(); i += 3) {
            auto index1 = unitCube.indices[i];
            auto index2 = unitCube.indices[i + 1];
            auto index3 = unitCube.indices[i + 2];

            auto vertex1 = unitCube.vertices[index1];
            auto vertex2 = unitCube.vertices[index2];
            auto vertex3 = unitCube.vertices[index3];

            auto u = vertex2 - vertex1;
            auto v = vertex3 - vertex1;

            glm::vec3 normal = {
                (u.y * v.z) - (u.z * v.y),
                (u.z * v.x) - (u.x * v.z),
                (u.x * v.y) - (u.y * v.x)
            };

            mesh.push_back(
                {
                    .position = vertex1,
                    .normal = normal
                }
            );

            mesh.push_back(
                {
                    .position = vertex2,
                    .normal = normal
                }
            );

            mesh.push_back(
                {
                    .position = vertex3,
                    .normal = normal
                }
            );
        }

        return mesh;
    }

    std::vector<glm::vec2> generateCircleMesh(int resolution) {
        std::vector<glm::vec2> circleVertices;

        for (int i = 0; i < resolution; i++) {
            float valueStart = (float) i / (float) resolution;
            float valueEnd = ((float) (i + 1) / (float) resolution);

            float angleStart = valueStart * glm::pi<float>() * 2.f;
            float angleEnd = valueEnd * glm::pi<float>() * 2.f;

            circleVertices.emplace_back(glm::cos(angleStart), glm::sin(angleStart));
            circleVertices.emplace_back(glm::cos(angleEnd), glm::sin(angleEnd));
            circleVertices.emplace_back(0.f, 0.f);
        }

        return std::move(circleVertices);
    }
}