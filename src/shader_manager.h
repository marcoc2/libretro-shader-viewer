#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader_processor.h"

class ShaderManager {
private:
    static GLuint compileShader(const char* source, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::" 
                      << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") 
                      << "::COMPILATION_FAILED\n" << infoLog << std::endl;
            std::cerr << "Shader source:\n" << source << std::endl;
            return 0;
        }
        return shader;
    }

public:
    static GLuint createProgram(const std::string& shaderSource) {
        // Processa o shader para separar vertex e fragment
        ProcessedShader processed = ShaderProcessor::process(shaderSource);

        // Compila os shaders
        GLuint vertexShader = compileShader(processed.vertexSource.c_str(), GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(processed.fragmentSource.c_str(), GL_FRAGMENT_SHADER);

        if (!vertexShader || !fragmentShader) {
            if (vertexShader) glDeleteShader(vertexShader);
            if (fragmentShader) glDeleteShader(fragmentShader);
            return 0;
        }

        // Cria e linka o programa
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

#endif // SHADER_MANAGER_H