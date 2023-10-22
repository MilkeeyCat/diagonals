#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "../opengl_helping_functions.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
        }

        return 0;
    }
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> elements;
        unsigned int stride;

    public:
        VertexBufferLayout() : stride(0) {}

        template<typename T>
        void Push(unsigned int count);

        const std::vector<VertexBufferElement> GetElements() const { return elements; }
        unsigned int GetStride() const { return stride; }

};

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
    elements.push_back({GL_FLOAT, count, false});
    stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_INT, count, false});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_BYTE, count, true});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
