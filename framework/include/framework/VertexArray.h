#ifndef PROG2002_VERTEXARRAY_H
#define PROG2002_VERTEXARRAY_H

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <optional>
#include <ranges>
#include <iostream>
#include "glad/glad.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

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
    
    /**
     * An vertex array object that can be drawed
     */
    template<typename VertexType>
    struct VertexArray {
        const std::shared_ptr<Shader> shader;

        VertexBuffer<VertexType> vertexBuffer;
        std::optional<IndexBuffer> indexBuffer;

        uint32_t vertexArrayId = 0;

        VertexArray(
            std::shared_ptr<Shader> shader,
            std::vector<VertexAttribute> attributes,
            VertexBuffer<VertexType> vertexBuffer,
            std::optional<IndexBuffer> indexBuffer
        ) : shader(std::move(shader)), vertexBuffer(std::move(vertexBuffer)), indexBuffer(std::move(indexBuffer)) {
            glCreateVertexArrays(1, &vertexArrayId);

            // Vertex attributes
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

            // Bind vertex buffer
            glVertexArrayVertexBuffer(
                vertexArrayId,
                0,
                this->vertexBuffer.vertexBufferId,
                0,
                sizeof(VertexType)
            );

            // Bind index buffer
            if (this->indexBuffer.has_value()) {
                glVertexArrayElementBuffer(vertexArrayId, this->indexBuffer->indexBufferId);
            }
        };

        // Move constructor
        VertexArray(VertexArray &&object) noexcept:
            shader(std::move(object.shader)),
            vertexBuffer(std::move(object.vertexBuffer)),
            indexBuffer(std::move(object.indexBuffer)),
            vertexArrayId(object.vertexArrayId) {
            object.vertexArrayId = 0;
        }

        ~VertexArray() {
            if (vertexArrayId) glDeleteVertexArrays(1, &vertexArrayId);
        }

        void draw(GLenum drawMode = GL_TRIANGLES) const {
            glUseProgram(shader->id);
            glBindVertexArray(vertexArrayId);

            if (indexBuffer.has_value()) {
                glDrawElements(drawMode, indexBuffer->elementsAmount, GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(drawMode, 0, vertexBuffer.verticesAmount);
            }
        }

        /**
         * Draw with given amount of instances, and use `gl_InstanceID` in shader to differentiate instances
         */
        void drawInstanced(uint32_t instances, GLenum drawMode = GL_TRIANGLES) const {
            glUseProgram(shader->id);
            glBindVertexArray(vertexArrayId);

            if (indexBuffer.has_value()) {
                glDrawElementsInstanced(drawMode, indexBuffer->elementsAmount, GL_UNSIGNED_INT, nullptr, instances);
            } else {
                glDrawArraysInstanced(drawMode, 0, vertexBuffer.verticesAmount, instances);
            }
        }
    };
}

#endif //PROG2002_VERTEXARRAY_H
