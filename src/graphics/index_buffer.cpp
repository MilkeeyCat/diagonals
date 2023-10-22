#include "index_buffer.h"
#include "../opengl_helping_functions.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, unsigned int usage): count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);
}

IndexBuffer::~IndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::UpdateData(const unsigned int *data, unsigned int size) const {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}
