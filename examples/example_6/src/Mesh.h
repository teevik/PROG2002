#ifndef PROG2002_MESH_H
#define PROG2002_MESH_H

#include <vector>
#include <cstdint>
#include "glad/glad.h"

template<typename VertexType>
struct Triangle {
    VertexType a;
    VertexType b;
    VertexType c;
};

struct VertexAttribute {
    /// The type of the data stored in the vertex
    GLenum type;

    /// The amount of values per vertex
    uint32_t size;

    /// Relative offset from the start of the vertex to this attribute
    uint32_t offset;

    /// `true` if the parameter represents a normalized integer (type must be an integer type). `false` otherwise.
    bool normalize;
};

template<typename VertexType>
struct GpuMesh {
    using TriangleType = Triangle<VertexType>;

    const std::vector<TriangleType> triangles;

    uint32_t vertexBufferId;
    uint32_t vertexArrayId;

    void draw() {
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void free() {
        glDeleteBuffers(1, &vertexBufferId);
        glDeleteVertexArrays(1, &vertexArrayId);
    }
};

template<typename VertexType>
struct Mesh {
    using TriangleType = Triangle<VertexType>;

    std::vector<VertexAttribute> attributes;
    std::vector<TriangleType> triangles;

    void pushTriangles(std::initializer_list<TriangleType> newTriangles) {
        triangles.insert(triangles.end(), newTriangles.begin(), newTriangles.end());
    }

    GpuMesh<VertexType> initialize() {
        GpuMesh<VertexType> gpuMesh{.triangles = std::move(this->triangles)};

        glCreateVertexArrays(1, &gpuMesh.vertexArrayId);
        glCreateBuffers(1, &gpuMesh.vertexBufferId);
        // glCreateBuffers(1, &indexBufferId);

        glNamedBufferData(
            gpuMesh.vertexBufferId,
            gpuMesh.triangles.size() * sizeof(TriangleType),
            gpuMesh.triangles.data(),
            GL_STATIC_DRAW
        );
        // glnamedBufferData(indexBufferId, sizeof(indices), GL_STATIC_DRAW);

        for (int attributeIndex = 0; attributeIndex < attributes.size(); ++attributeIndex) {
            VertexAttribute vertexAttribute = attributes[attributeIndex];

            glEnableVertexArrayAttrib(gpuMesh.vertexArrayId, attributeIndex);
            glVertexArrayAttribBinding(gpuMesh.vertexArrayId, attributeIndex, 0);
            glVertexArrayAttribFormat(
                gpuMesh.vertexArrayId,
                attributeIndex,
                vertexAttribute.size,
                vertexAttribute.type,
                vertexAttribute.normalize,
                vertexAttribute.offset
            );
        }

        glVertexArrayVertexBuffer(
            gpuMesh.vertexArrayId,
            0,
            gpuMesh.vertexBufferId,
            0,
            sizeof(VertexType)
        );

        return gpuMesh;
    }
};

#endif //PROG2002_MESH_H
