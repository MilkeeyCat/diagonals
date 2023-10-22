#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture(const std::string &path)
    : rendererId(0), filePath(path), buffer(nullptr), widht(0), height(0), BPP(0) {

    stbi_set_flip_vertically_on_load(1);
    buffer = stbi_load(path.c_str(),  &widht, &height, &BPP, 4);

    GL_CALL(glGenTextures(1, &rendererId));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererId));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, widht, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    if(buffer) {
        stbi_image_free(buffer);
    }
}

Texture::~Texture() {
    GL_CALL(glDeleteTextures(1, &rendererId));
}

void Texture::Bind(unsigned int slot) const {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererId));
}

void Texture::Unbind() const {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
