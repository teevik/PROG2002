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
        const std::vector<VertexType> vertices;
        const std::vector<IndexType> indices;

        const std::shared_ptr<Shader> shader;
        uint32_t vertexArrayId;
        uint32_t vertexBufferId;
        uint32_t indexBufferId;

        VertexArrayObject(
            const std::vector<VertexType> vertices,
            const std::vector<IndexType> &indices,
            const std::shared_ptr<Shader> &shader,
            uint32_t vertexArrayId,
            uint32_t vertexBufferId,
            uint32_t indexBufferId
        ) : vertices(vertices),
            indices(indices),
            shader(shader),
            vertexArrayId(vertexArrayId),
            vertexBufferId(vertexBufferId),
            indexBufferId(indexBufferId) {};

        VertexArrayObject(VertexArrayObject &&object) noexcept:
            vertices(std::move(object.vertices)),
            indices(std::move(object.indices)),
            shader(std::move(object.shader)),
            vertexArrayId(object.vertexArrayId),
            vertexBufferId(object.vertexBufferId),
            indexBufferId(object.indexBufferId) {
            object.vertexArrayId = 0;
            object.vertexBufferId = 0;
            object.indexBufferId = 0;
        }

        ~VertexArrayObject() {
            if (vertexBufferId) glDeleteBuffers(1, &vertexBufferId);
            if (indexBufferId) glDeleteBuffers(1, &indexBufferId);
            if (vertexArrayId) glDeleteVertexArrays(1, &vertexArrayId);
        }

        void draw(GLenum drawMode = GL_TRIANGLES) const {
            glUseProgram(shader->id);
            glBindVertexArray(vertexArrayId);
            glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, nullptr);
        }

        void drawInstanced(uint32_t instances, GLenum drawMode = GL_TRIANGLES) const {
            glUseProgram(shader->id);
            glBindVertexArray(vertexArrayId);
            glDrawElementsInstanced(drawMode, indices.size(), GL_UNSIGNED_INT, nullptr, instances);
        }

        /**
        * Creates a VertexArrayObject, using the following:
        * - One shader
        * - List of vertex attributes
        * - Vertices
        * - Optionally indices, otherwise autogenerated
        */
        static VertexArrayObject<VertexType> create(
            std::shared_ptr<Shader> shader,
            std::vector<VertexAttribute> attributes,
            std::vector<VertexType> vertices,
            std::optional<std::vector<IndexType>> indices
        ) {
            uint32_t vertexArrayId;
            uint32_t vertexBufferId;
            uint32_t indexBufferId;

            glCreateVertexArrays(1, &vertexArrayId);
            glCreateBuffers(1, &vertexBufferId);
            glCreateBuffers(1, &indexBufferId);

            // Generate indices if not given
            std::vector<IndexType> indicesOrGenerated;
            if (indices.has_value()) {
                indicesOrGenerated = std::move(indices.value());
            } else {
                auto indexGenerator = std::views::iota(0u, (IndexType) vertices.size());
                indicesOrGenerated = {indexGenerator.begin(), indexGenerator.end()};
            }

            VertexArrayObject<VertexType> builtObject(
                std::move(vertices),
                std::move(indicesOrGenerated),
                shader,
                vertexArrayId,
                vertexBufferId,
                indexBufferId
            );

            // Vertex buffer
            glNamedBufferData(
                vertexBufferId,
                builtObject.vertices.size() * sizeof(VertexType),
                builtObject.vertices.data(),
                GL_STATIC_DRAW
            );

            for (int attributeIndex = 0; attributeIndex < attributes.size(); ++attributeIndex) {
                VertexAttribute vertexAttribute = attributes[attributeIndex];

                glEnableVertexArrayAttrib(vertexArrayId, attributeIndex);
                glVertexArrayAttribBinding(vertexArrayId, attributeIndex, 0);
                glVertexArrayAttribFormat(
                    vertexArrayId,
                    attributeIndex,
                    (int32_t) vertexAttribute.size,
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
