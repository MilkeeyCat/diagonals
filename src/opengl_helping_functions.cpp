#include <GL/glew.h>
#include <iostream>

void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function_name, const char *file, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "Open GL error lel: (0x" << std::hex << error << "): " << file << ": " << function_name << ":" << std::dec << line << std::endl;
        return false;
    }

    return true;
}
