#pragma once

#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer_layout.h"
#include "graphics/vertex_array.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "text_renderer.h"
#include "utils.h"

class Component {
public:
    Component() {};
    virtual ~Component() {}

    virtual void Render() {}
    virtual void Update() {}
    virtual void OnClick(double x, double y) {}
    virtual void OnMouseMove(double x, double y) {}
    virtual void OnKeyPress(int key, int scancode, int action, int mods) {}
    virtual void Reset() {}
protected:

    VertexBuffer *vertexBuffer;
    VertexArray *vertexArray;
    IndexBuffer *indexBuffer;
    Texture *texture;
    Shader *shader;
};
