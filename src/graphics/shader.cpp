#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"
#include "../opengl_helping_functions.h"

Shader::Shader(const std::string &filepath)
    : filePath(filepath), rendererId(0) {
        ShaderProgramSource source = ParseShader(filepath);
        rendererId = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    GL_CALL(glDeleteProgram(rendererId));
}

ShaderProgramSource Shader::ParseShader(const std::string& path) {
    std::fstream stream(path);
    std::string line;
    std::stringstream ss[2];

    enum {
        VERTEX = 0,
        FRAGMENT = 1,
        NONE = -1
    } mode = NONE;

    while (getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                mode = VERTEX;
            } else if(line.find("fragment") != std::string::npos) {
                mode = FRAGMENT;
            }
        } else {
            ss[(int)mode] << line << "\n";
        }
    }

    return ShaderProgramSource { ss[0].str(), ss[1].str() };
}

GLuint Shader::CompileShader(unsigned int type, std::string &source) {
    GLuint id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int len;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        char *val = new char[len];

        glGetShaderInfoLog(id, len, &len, val);

        std::cout << "You dumb fuck " << val;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

GLuint Shader::CreateShader(std::string &vertexShader, std::string &fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const {
    GL_CALL(glUseProgram(rendererId));
}

void Shader::Unbind() const {
    GL_CALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int value) {
    GL_CALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1iv(const std::string &name, unsigned int count, int *value) {
    GL_CALL(glUniform1iv(GetUniformLocation(name), count, value));
}

int Shader::GetUniformLocation(const std::string &name) {
    if(uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }

    GL_CALL(int location = glGetUniformLocation(rendererId, name.c_str()));
    if(location == -1) {
        std::cout << "Someone has a -1 location, zamn... " << name << std::endl;
    } else {
        uniformLocationCache[name] = location;
    }

    return location;
}
