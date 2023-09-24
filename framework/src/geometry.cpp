#include "framework/geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

namespace framework {
    std::vector<framework::Triangle<glm::vec2>> generateCircleMesh(int resolution) {
        std::vector<framework::Triangle<glm::vec2>> circleTriangles;

        for (int i = 0; i < resolution; i++) {
            float valueStart = (float) i / (float) resolution;
            float valueEnd = ((float) (i + 1) / (float) resolution);

            float angleStart = valueStart * glm::pi<float>() * 2.f;
            float angleEnd = valueEnd * glm::pi<float>() * 2.f;

            circleTriangles.push_back(
                {
                    .a = glm::vec2(glm::cos(angleStart), glm::sin(angleStart)),
                    .b = glm::vec2(glm::cos(angleEnd), glm::sin(angleEnd)),
                    .c = glm::vec2(0.f, 0.f)
                }
            );
        }

        return std::move(circleTriangles);
    }
}