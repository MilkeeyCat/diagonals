#include "vertex_array.h"
#include "vertex_buffer_layout.h"

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &rendererId));
}

VertexArray::~VertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::Bind() const {
    GL_CALL(glBindVertexArray(rendererId));
}

void VertexArray::Unbind() const {
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;

    for(unsigned int i = 0; i < elements.size(); i++) {
        const auto &element = elements[i];

        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}
