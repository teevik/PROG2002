#ifndef PROG2002_CAMERA_H
#define PROG2002_CAMERA_H

#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace framework {
    struct Camera {
        glm::mat4 projectionMatrix;

        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;

        static Camera createOrthographic(
            float size,
            float aspectRatio,
            glm::vec3 position,
            glm::vec3 target,
            glm::vec3 up,
            float zNear = -0.01f,
            float zFar = 1.0f
        );

        static Camera createPerspective(
            float fov,
            float aspectRatio,
            glm::vec3 position,
            glm::vec3 target,
            glm::vec3 up,
            float zNear = 1.f,
            float zFar = -10.f
        );

        [[nodiscard]] glm::mat4 viewMatrix() const;
    };
}

#endif //PROG2002_CAMERA_H
