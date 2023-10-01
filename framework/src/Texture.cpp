#include <iostream>
#include "framework/Texture.h"
#include "glad/glad.h"

namespace framework {
    void Texture::free() const {
        glDeleteTextures(1, &id);
        stbi_image_free(pixels);
    }

    void Texture::bind() const {
        glBindTextureUnit(0, id);
    }

    Texture loadTexture(
        const std::string &path,
        Filtering filtering,
        Wrapping wrapping
    ) {
        int imageWidth, imageHeight, bpp;
        auto pixels = stbi_load(path.c_str(), &imageWidth, &imageHeight, &bpp, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load texture");
        }

        uint32_t textureId;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

        int filteringInt;
        switch (filtering) {
            case Filtering::Nearest:
                filteringInt = GL_NEAREST;
                break;

            case Filtering::Linear:
                filteringInt = GL_LINEAR;
                break;
        }

        // Filtering
        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, filteringInt);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, filteringInt);

        int wrappingInt;
        switch (wrapping) {
            case Wrapping::Repeat:
                wrappingInt = GL_REPEAT;
                break;
        }

        // Wrapping
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrappingInt);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrappingInt);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, wrappingInt);

        glTextureStorage2D(textureId, 1, GL_RGBA8, imageWidth, imageHeight);
        glTextureSubImage2D(textureId, 0, 0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        return Texture{
            .id = textureId,
            .pixels = pixels
        };
    }

    Texture loadCubemap(
        const std::string &path,
        Filtering filtering,
        Wrapping wrapping
    ) {
        int imageWidth, imageHeight, bpp;
        auto pixels = stbi_load(path.c_str(), &imageWidth, &imageHeight, &bpp, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load texture");
        }

        uint32_t textureId;
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureId);

        int filteringInt;
        switch (filtering) {
            case Filtering::Nearest:
                filteringInt = GL_NEAREST;
                break;

            case Filtering::Linear:
                filteringInt = GL_LINEAR;
                break;
        }

        // Filtering
        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, filteringInt);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, filteringInt);

        int wrappingInt;
        switch (wrapping) {
            case Wrapping::Repeat:
                wrappingInt = GL_REPEAT;
                break;
        }

        // Wrapping
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, wrappingInt);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, wrappingInt);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, wrappingInt);

        glTextureStorage2D(textureId, 1, GL_RGBA8, imageWidth, imageHeight);
        for (int i = 0; i < 6; ++i) {
            glTextureSubImage3D(textureId, 0, 0, 0, i, imageWidth, imageHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }

        return Texture{
            .id = textureId,
            .pixels = pixels
        };
    }
}