#ifndef PROG2002_VERTEXARRAYOBJECT_H
#define PROG2002_VERTEXARRAYOBJECT_H

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <optional>
#include <ranges>
#include <iostream>
#include "glad/glad.h"
#include "Shader.h"

namespace framework {
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

    using IndexType = uint32_t;

    /**
     * An vertex array object that can be drawed
     */
    template<typename VertexType>
    struct VertexArrayObject {
        using TriangleType = Triangle<VertexType>;

        const std::vector<TriangleType> triangles;
        const std::vector<IndexType> indices;

        const std::shared_ptr<Shader> shader;
        const uint32_t vertexArrayId;
        const uint32_t vertexBufferId;
        const uint32_t indexBufferId;

        void draw() {
            glUseProgram(shader->id);
            glBindVertexArray(vertexArrayId);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        }

        void free() {
            glDeleteBuffers(1, &vertexBufferId);
            glDeleteBuffers(1, &indexBufferId);
            glDeleteVertexArrays(1, &vertexArrayId);
        }
    };

    /**
     * A builder for VertexArrayObject, contains the following:
     * - One shader
     * - One list of vertex attributes
     * - Multiple static meshes
     */
    template<typename VertexType>
    struct VertexArrayObjectBuilder {
        using TriangleType = Triangle<VertexType>;

        std::shared_ptr<Shader> shader;
        std::vector<VertexAttribute> attributes;
        std::vector<TriangleType> triangles;
        std::optional<std::vector<IndexType>> indices;

        VertexArrayObject<VertexType> build() {
            uint32_t vertexArrayId;
            uint32_t vertexBufferId;
            uint32_t indexBufferId;

            glCreateVertexArrays(1, &vertexArrayId);
            glCreateBuffers(1, &vertexBufferId);
            glCreateBuffers(1, &indexBufferId);

            // TODO: Should only generate indices if indices arent provided, some sort of lazy way?
            auto indexGenerator = std::views::iota(0u, (IndexType) triangles.size() * 3);
            std::vector<IndexType> generatedIndices = {indexGenerator.begin(), indexGenerator.end()};

            VertexArrayObject<VertexType> builtObject{
                .triangles = std::move(triangles),
                .indices = std::move(indices.value_or(generatedIndices)),
                .shader = shader,
                .vertexArrayId = vertexArrayId,
                .vertexBufferId = vertexBufferId,
                .indexBufferId = indexBufferId
            };

            // Vertex buffer
            glNamedBufferData(
                vertexBufferId,
                builtObject.triangles.size() * sizeof(TriangleType),
                builtObject.triangles.data(),
                GL_STATIC_DRAW
            );

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

            // Index buffer
            glNamedBufferData(
                indexBufferId,
                builtObject.indices.size() * sizeof(IndexType),
                builtObject.indices.data(),
                GL_STATIC_DRAW
            );

            glVertexArrayElementBuffer(vertexArrayId, indexBufferId);

            return builtObject;
        }
    };
}


#endif //PROG2002_VERTEXARRAYOBJECT_H
