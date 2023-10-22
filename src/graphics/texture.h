#pragma once

#include "../opengl_helping_functions.h"
#include <iostream>

class Texture {
    private:
        unsigned int rendererId;
        std::string filePath;
        unsigned char *buffer;
        int widht, height, BPP;
    public:
        Texture(const std::string &path);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        int GetWidth() const { return widht; };
        int GetHeight() const { return height; };

        unsigned int GetRendererId() const { return rendererId; };
};
