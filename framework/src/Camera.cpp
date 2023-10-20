#include "framework/Camera.h"

namespace framework {
    Camera Camera::createOrthographic(
        float size,
        float aspectRatio,
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 up,
        float zNear,
        float zFar
    ) {
        auto projectionMatrix = glm::ortho(-size * aspectRatio, size * aspectRatio, -size, size, zNear, zFar);

        Camera camera = {
            .projectionMatrix = projectionMatrix,
            .position = position,
            .target = target,
            .up = up
        };
        
        return camera;
    }

    Camera Camera::createPerspective(
        float fov,
        float aspectRatio,
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 up,
        float zNear,
        float zFar
    ) {
        auto projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);

        Camera camera = {
            .projectionMatrix = projectionMatrix,
            .position = position,
            .target = target,
            .up = up
        };

        return camera;
    }


    glm::mat4 Camera::viewMatrix() const {
        return glm::lookAt(
            position,
            target,
            up
        );
    }
}
