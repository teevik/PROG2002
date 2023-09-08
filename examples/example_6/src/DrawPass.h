#ifndef PROG2002_DRAWPASS_H
#define PROG2002_DRAWPASS_H

#include <vector>
#include <cstdint>
#include <string>
#include "glad/glad.h"
#include "shader.h"

template<typename VertexType>
struct Triangle {
    VertexType a;
    VertexType b;
    VertexType c;
};

/**
 * Specifies layout of a specific vertex attribute
 */
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


/// A mesh that can't change
template<typename VertexType>
struct StaticMesh {
    using TriangleType = Triangle<VertexType>;

    const std::vector<TriangleType> triangles;
};

/**
 * An initialized version of `DrawPass` with all OpenGL objects created
 */
template<typename VertexType>
struct InitializedDrawPass {
    using TriangleType = Triangle<VertexType>;

    const std::vector<TriangleType> triangles;

    const uint32_t shaderId;
    const uint32_t vertexBufferId;
    const uint32_t vertexArrayId;

    void draw() {
        glUseProgram(shaderId);
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void free() {
        glDeleteBuffers(1, &vertexBufferId);
        glDeleteVertexArrays(1, &vertexArrayId);
        glDeleteProgram(shaderId);
    }
};

/**
 * A draw pass, contains the following:
 * - One shader pipeline
 * - One list of vertex attributes
 * - Multiple static meshes
 */
template<typename VertexType>
struct DrawPass {
    using TriangleType = Triangle<VertexType>;
    using StaticMeshType = StaticMesh<VertexType>;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    std::vector<VertexAttribute> attributes;
    std::vector<StaticMeshType> staticMeshes;

    InitializedDrawPass<VertexType> initialize() {
        std::vector<TriangleType> triangles;
        for (auto &staticMesh: staticMeshes) {
            triangles.insert(triangles.end(), staticMesh.triangles.begin(), staticMesh.triangles.end());
        }

        uint32_t shaderId = createShaderPipeline(vertexShaderSource, fragmentShaderSource);

        uint32_t vertexArrayId;
        uint32_t vertexBufferId;

        glCreateVertexArrays(1, &vertexArrayId);
        glCreateBuffers(1, &vertexBufferId);
        // glCreateBuffers(1, &indexBufferId);

        InitializedDrawPass<VertexType> initializedDrawPass{
            .triangles = std::move(triangles),
            .shaderId = shaderId,
            .vertexBufferId = vertexBufferId,
            .vertexArrayId = vertexArrayId
        };

        glNamedBufferData(
            vertexBufferId,
            initializedDrawPass.triangles.size() * sizeof(TriangleType),
            initializedDrawPass.triangles.data(),
            GL_STATIC_DRAW
        );
        // glnamedBufferData(indexBufferId, sizeof(indices), GL_STATIC_DRAW);

        for (int attributeIndex = 0; attributeIndex < attributes.size(); ++attributeIndex) {
            VertexAttribute vertexAttribute = attributes[attributeIndex];

            glEnableVertexArrayAttrib(vertexArrayId, attributeIndex);
            glVertexArrayAttribBinding(vertexArrayId, attributeIndex, 0);
            glVertexArrayAttribFormat(
                vertexArrayId,
                attributeIndex,
                vertexAttribute.size,
                vertexAttribute.type,
                vertexAttribute.normalize,
                vertexAttribute.offset
            );
        }

        glVertexArrayVertexBuffer(
            vertexArrayId,
            0,
            vertexBufferId,
            0,
            sizeof(VertexType)
        );

        return initializedDrawPass;
    }
};

#endif //PROG2002_DRAWPASS_H
