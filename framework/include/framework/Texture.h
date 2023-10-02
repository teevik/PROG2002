#ifndef PROG2002_TEXTURE_H
#define PROG2002_TEXTURE_H

#include <cstdint>
#include <string>
#include "stb_image.h"

namespace framework {
    enum class Filtering {
        Nearest,
        Linear
    };

    enum class Wrapping {
        Repeat
    };

    class Texture {
    private:
        uint32_t id;
        stbi_uc *pixels;

    public:
        Texture(uint32_t id, stbi_uc *pixels);

        Texture(Texture &&texture) noexcept;

        ~Texture();

        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        void bind() const;
    };

    Texture loadTexture(
        const std::string &path,
        Filtering filtering = Filtering::Linear,
        Wrapping wrapping = Wrapping::Repeat
    );

    Texture loadCubemap(
        const std::string &path,
        Filtering filtering = Filtering::Linear,
        Wrapping wrapping = Wrapping::Repeat
    );
}

#endif //PROG2002_TEXTURE_H
