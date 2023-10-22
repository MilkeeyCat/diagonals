#include "vertex_buffer.h"
#include <GL/glew.h>
#include "../opengl_helping_functions.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, unsigned int usage) {
    glGenBuffers(1, &rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    GL_CALL(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::UpdateData(const void *data, unsigned int size) const {
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
