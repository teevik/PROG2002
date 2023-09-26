#include "framework/geometry.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

namespace framework {
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