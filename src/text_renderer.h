#pragma once

#include "glm/glm.hpp"
#include <string>
#include <GL/glew.h>

#define GLT_IMPLEMENTATION
#include "glText/gltext.h"

struct Text {
    glm::vec2 pos;
    glm::vec4 color;
    GLTtext *text;
    float scale;
};

class TextRender {
public:
    TextRender();

    void Render(Text &text, int horizontal_aligment = GLT_LEFT, int vertical_aligment = GLT_RIGHT);
    GLTtext *Create(const char *string);
};
