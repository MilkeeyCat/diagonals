#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
    private:
        unsigned int rendererId;
        std::string filePath;
        std::unordered_map<std::string, int> uniformLocationCache;

    public:
        Shader(const std::string &filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniform1i(const std::string &name, int value);
        void SetUniform1iv(const std::string &name, unsigned int count, int *value);
        void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

    private:
        ShaderProgramSource ParseShader(const std::string& path);
        unsigned int CompileShader(unsigned int type, std::string &source);
        unsigned int CreateShader(std::string &vertexShader, std::string &fragmentShader);
        int GetUniformLocation(const std::string &name);
};
