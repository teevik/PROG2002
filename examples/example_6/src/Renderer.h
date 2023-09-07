#ifndef PROG2002_RENDERER_H
#define PROG2002_RENDERER_H

#include <cstdint>
#include <vector>
#include "glm/mat4x4.hpp"

const size_t MAX_TRIANGLES = 2048;

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

struct Triangle {
    Vertex a;
    Vertex b;
    Vertex c;
};

struct Renderer {
private:
    uint32_t vertexArrayId;
    uint32_t vertexBufferId;
    uint32_t shaderId;

    glm::mat4 projection;
    std::vector<Triangle> triangles;

public:
    void initalize();

    void free() const;

    void beginFrame();

    void endFrame();

    void pushTriangle(Triangle triangle);
};

#endif //PROG2002_RENDERER_H
