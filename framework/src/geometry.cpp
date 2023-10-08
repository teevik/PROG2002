#include <unordered_map>
#include "framework/geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

namespace framework {
    std::vector<VertexWithNormal> generateUnitCubeWithNormals() {
        std::vector<VertexWithNormal> mesh;

        for (int i = 0; i < unitCube::indices.size(); i += 3) {
            auto index1 = unitCube::indices[i];
            auto index2 = unitCube::indices[i + 1];
            auto index3 = unitCube::indices[i + 2];

            auto vertex1 = unitCube::vertices[index1];
            auto vertex2 = unitCube::vertices[index2];
            auto vertex3 = unitCube::vertices[index3];

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

    IndexMesh generateGridMesh(int resolution) {
        std::vector<glm::vec2> vertices;

        for (int y = 0; y <= resolution; ++y) {
            for (int x = 0; x <= resolution; ++x) {
                glm::vec2 vertex = {((float) x / (float) resolution) - 0.5f, ((float) y / (float) resolution) - 0.5f};

                vertices.push_back(vertex);
            }
        }

        std::vector<uint32_t> indices;

        for (int y = 0; y < resolution; ++y) {
            for (int x = 0; x < resolution; ++x) {
                uint32_t i = y * (resolution + 1) + x;

                uint32_t topLeft = i;
                uint32_t topRight = i + 1;
                uint32_t bottomLeft = i + resolution + 1;
                uint32_t bottomRight = i + 1 + resolution + 1;

                // Triangle 1
                indices.push_back(topRight);
                indices.push_back(bottomRight);
                indices.push_back(topLeft);

                // Triangle 2
                indices.push_back(bottomRight);
                indices.push_back(bottomLeft);
                indices.push_back(topLeft);
            }
        }

        return {
            .vertices = vertices,
            .indices = indices
        };
    }
}