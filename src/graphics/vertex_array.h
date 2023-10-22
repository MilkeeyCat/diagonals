#pragma once

#include "../opengl_helping_functions.h"
#include "vertex_buffer.h"

class VertexBufferLayout;

class VertexArray {
    private:
        unsigned int rendererId;
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;
        void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};
