#ifndef PROG2002_OBJECT_H
#define PROG2002_OBJECT_H

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include "glad/glad.h"
#include "Shader.h"

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
 * An object that can be drawed
 */
template<typename VertexType>
struct Object {
    using TriangleType = Triangle<VertexType>;

    const std::vector<TriangleType> triangles;

    const std::shared_ptr<Shader> shader;
    const uint32_t vertexBufferId;
    const uint32_t vertexArrayId;

    void draw() {
        glUseProgram(shader->id);
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void free() {
        glDeleteBuffers(1, &vertexBufferId);
        glDeleteVertexArrays(1, &vertexArrayId);
    }
};

/**
 * A builder for object, contains the following:
 * - One shader
 * - One list of vertex attributes
 * - Multiple static meshes
 */
template<typename VertexType>
struct ObjectBuilder {
    using TriangleType = Triangle<VertexType>;
    using StaticMeshType = StaticMesh<VertexType>;

    std::shared_ptr<Shader> shader;
    std::vector<VertexAttribute> attributes;
    std::vector<StaticMeshType> staticMeshes;

    Object<VertexType> build() {
        std::vector<TriangleType> triangles;
        for (auto &staticMesh: staticMeshes) {
            triangles.insert(triangles.end(), staticMesh.triangles.begin(), staticMesh.triangles.end());
        }

        uint32_t vertexArrayId;
        uint32_t vertexBufferId;

        glCreateVertexArrays(1, &vertexArrayId);
        glCreateBuffers(1, &vertexBufferId);
        // glCreateBuffers(1, &indexBufferId);

        Object<VertexType> builtObject{
            .triangles = std::move(triangles),
            .shader = shader,
            .vertexBufferId = vertexBufferId,
            .vertexArrayId = vertexArrayId
        };

        glNamedBufferData(
            vertexBufferId,
            builtObject.triangles.size() * sizeof(TriangleType),
            builtObject.triangles.data(),
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

        return builtObject;
    }
};

#endif //PROG2002_OBJECT_H
