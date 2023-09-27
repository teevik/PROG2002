#ifndef PROG2002_TEXTURE_H
#define PROG2002_TEXTURE_H

#include <cstdint>
#include <string>
#include "stb_image.h"

struct Texture {
    uint32_t id;
    stbi_uc *pixels;

    void free() const;

    void bind() const;
};

enum class Filtering {
    Nearest,
    Linear
};

enum class Wrapping {
    Repeat
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


#endif //PROG2002_TEXTURE_H
